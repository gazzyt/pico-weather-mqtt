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
    void DisplayText(const std::string& line1, const std::string& line2);

private:
    static constexpr size_t HEIGHT = 32;
    static constexpr size_t WIDTH = 128;
    static constexpr size_t PAGE_HEIGHT = 8;
    static constexpr size_t NUM_PAGES = HEIGHT / PAGE_HEIGHT;
    static constexpr size_t BUF_LEN = NUM_PAGES * WIDTH;

    uint8_t m_buffer[BUF_LEN];
    
    // Initialize render area for entire frame (SSD1306_WIDTH pixels by SSD1306_NUM_PAGES pages)
    struct render_area m_frame_area = {
        start_col: 0,
        end_col : WIDTH - 1,
        start_page : 0,
        end_page : NUM_PAGES - 1
    };
};

#endif
