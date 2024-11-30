#include <iomanip>
#include <sstream>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mqtt_client.h"
#include "sensor/BMP085.h"
#include "sensor/DHT22.h"
#include "sensor/VEML6070.h"
#include "ssd1306_i2c.h"
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

void publish_temperature(float temp)
{
    WifiConnection wifi;
    MqttClient mqttClient{"rack2.mynet"};
    mqttClient.Publish("sensors/temperature", "wibble");
}

int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    SSD1306I2C display;
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


    while (true) {
        std::ostringstream sbuff1, sbuff2, sbuff3;

        veml6070.sleep(false);
        uint16_t uv = veml6070.readUV();
        sbuff1 << "UV " << uv;
        veml6070.sleep(true);

        bool dhtStatus = dht.read(false);

        if (dhtStatus)
        {
            sbuff2 << "T " << static_cast<int>(dht.readTemperature(false, false)) << " H " << static_cast<int>(dht.readHumidity(false));
        }

        auto bmpTemp = bmp085.readTemperature();
        sbuff3 << "T " << std::setprecision(3) << bmpTemp << " P " << bmp085.readPressure();

        display.DisplayText(sbuff1.str(), sbuff2.str(), sbuff3.str());
        printf(sbuff1.str().c_str());
        printf("\n");
        printf(sbuff2.str().c_str());
        printf("\n");
        printf(sbuff3.str().c_str());
        printf("\n");

        publish_temperature(bmpTemp);

        sleep_ms(10 * 60 * 1000);
    }
}
