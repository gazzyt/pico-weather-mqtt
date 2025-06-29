#ifndef SSD1306I2C_H
#define SSD1306I2C_H

#include <string>

struct render_area {
    uint8_t start_col;
    uint8_t end_col;
    uint8_t start_page;
    uint8_t end_page;

    int buflen;
};

class SSD1306I2C
{
public:
    SSD1306I2C();
    [[nodiscard]] inline bool IsDisplayPresent() const { return m_displayPresent; }
    void DisplayText(const std::string& line1, const std::string& line2, const std::string& line3);

private:
    static void SSD1306_init();
    static void WriteChar(uint8_t *buf, int16_t x, int16_t y, uint8_t ch);
    static void WriteString(uint8_t *buf, int16_t x, int16_t y, const char *str);
    static consteval uint8_t get_pin_config(uint8_t height, uint8_t width);

private:
    // Define the size of the display we have attached. This can vary, make sure you
    // have the right size defined or the output will look rather odd!
    // Code has been tested on 128x32 and 128x64 OLED displays
    static constexpr uint8_t HEIGHT = 32;
    static constexpr uint8_t WIDTH = 128;
    static constexpr uint8_t PAGE_HEIGHT = 8;
    static constexpr uint8_t PIN_CFG_128_32 = 0x02;
    static constexpr uint8_t PIN_CFG_128_64 = 0x12;
    static constexpr size_t NUM_PAGES = HEIGHT / PAGE_HEIGHT;
    static constexpr size_t BUF_LEN = NUM_PAGES * WIDTH;

    bool m_displayPresent;
    uint8_t m_buffer[BUF_LEN];
    
    // Initialize render area for entire frame (SSD1306_WIDTH pixels by SSD1306_NUM_PAGES pages)
    struct render_area m_frame_area = {
        .start_col = 0,
        .end_col = WIDTH - 1,
        .start_page = 0,
        .end_page = NUM_PAGES - 1,
        .buflen = 0
    };

};

#endif
