#include "hardware/timer.h"
#include "logger.h"
#include <chrono>
#include <cstdarg>
#include <cstdio>

Logger::log_levels Logger::log_level = info;

void Logger::Log(Logger::log_levels level, const char* format, ...)
{
    if (level >= log_level)
    {
        std::chrono::milliseconds ms{time_us_64() / 1000};
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(ms);
        ms -= std::chrono::duration_cast<std::chrono::milliseconds>(secs);
        auto mins = std::chrono::duration_cast<std::chrono::minutes>(secs);
        secs -= std::chrono::duration_cast<std::chrono::seconds>(mins);
        auto hours = std::chrono::duration_cast<std::chrono::hours>(mins);
        mins -= std::chrono::duration_cast<std::chrono::minutes>(hours);
        printf("%s %llu:%02llu:%02llu.%03llu ", level_names[level], hours.count(), mins.count(), secs.count(), ms.count());
        std::va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}
