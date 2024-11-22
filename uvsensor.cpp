#include <sstream>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "sensor/DHT22.h"
#include "sensor/VEML6070.h"
#include "ssd1306_i2c.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5
#define GPIO_DHT_PIN 14

VEML6070 veml6070;
DHT dht(GPIO_DHT_PIN, DHT11);

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

    veml6070.begin(I2C_PORT, VEML6070_4_T);
    dht.begin();
    SSD1306I2C display;

    while (true) {
        std::ostringstream sbuff1, sbuff2;

        veml6070.sleep(false);
        uint16_t uv = veml6070.readUV();
        sbuff1 << "UV " << uv;
        veml6070.sleep(true);

        bool dhtStatus = dht.read(false);

        if (dhtStatus)
        {
            sbuff2 << "T " << static_cast<int>(dht.readTemperature(false, false)) << " H " << static_cast<int>(dht.readHumidity(false));
        }

        display.DisplayText(sbuff1.str(), sbuff2.str());
        printf(sbuff1.str().c_str());
        printf(sbuff2.str().c_str());
        sleep_ms(5000);
    }
}
