#include <iomanip>
#include <limits>
#include <sstream>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "logger.h"
#include "memory_status.h"
#include "mqtt_client.h"
#include "power_status.h"
#include "sensor/BMP085.h"
#include "sensor/DHT22.h"
#include "sensor/VEML6070.h"
#include "sensor_values.h"
#include "ssd1306_i2c.h"
#include "watchdog.h"
#include "wifi_connection.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5
#define GPIO_DHT_PIN 14

VEML6070 veml6070;
DHT dht(GPIO_DHT_PIN, DHT11);
BMP085 bmp085;

void publish_sensor_values(SensorValues& values)
{
    WifiConnection wifi;
    float voltage;
    if (power_voltage(&voltage) == PICO_OK)
    {
        values.vsys = voltage;
    }

    const auto mqttPayload = values.to_json();
    LOG_INFO("MQTT payload: %s\n", mqttPayload.c_str());
    MqttClient mqttClient{"rack2.mynet"};
    mqttClient.Publish("sensors/station1", mqttPayload);
}

int main()
{
    unsigned int cycleNumber = 1;
    stdio_init_all();

    if (watchdog_caused_reboot())
    {
        LOG_INFO("Rebooted by Watchdog!\n");
    }
    else
    {
        LOG_INFO("Clean boot\n");
    }

    adc_init();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    SSD1306I2C display;
    if (display.IsDisplayPresent())
    {
        LOG_INFO("SSD1306 display present\n");
    }
    else
    {
        LOG_INFO("SSD1306 display NOT present\n");
    }

    veml6070.begin(I2C_PORT, VEML6070_4_T);
    dht.begin();
    if (!bmp085.begin(I2C_PORT))
    {
        display.DisplayText("BMP085 failed", "", "");
        sleep_ms(5000);
    }
    else
    {
        display.DisplayText("BMP085 succeeded", "", "");
        sleep_ms(5000);
    }

    Watchdog::enable();

    while (true) {
        LOG_INFO("Starting cycle %u\n", cycleNumber);
        
        MemoryStatus::PrintMemoryStatus();
        
        SensorValues values;
        values.cycle = cycleNumber++;

        veml6070.sleep(false);
        values.uv = veml6070.readUV();
        veml6070.sleep(true);

        if (values.uv == std::numeric_limits<decltype(values.uv)>::max())
        {
            // Have seen this happen occasionally due to i2c error.
            // In this case just drop readings - it'll work on the next cycle
            LOG_ERROR("Failed to read uv sensor\n");
        }
        else
        {
            bool dhtStatus = dht.read(false);

            if (dhtStatus)
            {
                values.temp1 = dht.readTemperature(false, false);
                values.humid = dht.readHumidity(false);
            }

            values.temp2 = bmp085.readTemperature();
            values.press = bmp085.readPressure();

            if (display.IsDisplayPresent())
            {
                std::ostringstream sbuff1, sbuff2, sbuff3;
                sbuff1 << "UV " << values.uv;
                sbuff2 << "T " << static_cast<int>(values.temp1) << " H " << static_cast<int>(values.humid);
                sbuff3 << "T " << std::setprecision(3) << values.temp2 << " P " << values.press;
                display.DisplayText(sbuff1.str(), sbuff2.str(), sbuff3.str());
            }

            watchdog_update();
            publish_sensor_values(values);
        }

        watchdog_update();

        for (auto i = 0; i < 75; ++i)
        {
            sleep_ms(8 * 1000);
            watchdog_update();
        }
    }
}
