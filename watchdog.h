#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "hardware/watchdog.h"
#include <cstdint>

class Watchdog
{
public:
    inline static constexpr uint32_t watchdog_reboot_time_ms = 8388;
    inline static constexpr uint32_t maximum_watchdog_reboot_time_ms = 8388;
    inline static constexpr uint32_t watchdog_time_ms = watchdog_reboot_time_ms - 300;
    Watchdog() = delete;

    inline static void enable()
    {
        watchdog_enable(watchdog_reboot_time_ms, true);
    }
    static_assert(watchdog_reboot_time_ms <= maximum_watchdog_reboot_time_ms);
    static_assert(watchdog_time_ms < watchdog_reboot_time_ms);
};


#endif
