#ifndef LOGGER_H
#define LOGGER_H

#include "hardware/timer.h"
#include <array>
#include <chrono>
#include <format>
#include <iostream>

class Logger
{
public:
    Logger() = delete;
    enum log_levels
    {
        debug = 0,
        info,
        warn,
        error
    };
    static log_levels log_level;

    template <typename ...Args>
    static void Log(log_levels level, std::format_string<Args...>&& format_string, Args&&... args)
    {
        if (level >= log_level)
        {
            std::chrono::milliseconds elapsed_time{time_us_64() / 1000};
            std::string timestamp = std::format(" {0:%T} ", elapsed_time);
            std::string message = std::format(format_string, std::forward<Args>(args)...);
            std::cout << level_names[level] << timestamp << message << std::endl;
        }
    }



private:
    static constexpr std::array<const char*, 4> level_names {"DBG", "INF", "WRN", "ERR"};
};

template <typename ...Args>
static void LogDebug(std::format_string<Args...> format_string, Args&&... args)
{
    Logger::Log(Logger::debug, std::forward<std::format_string<Args...> >(format_string), std::forward<Args>(args)...);
}

template <typename ...Args>
static void LogInfo(std::format_string<Args...> format_string, Args&&... args)
{
    Logger::Log(Logger::info, std::forward<std::format_string<Args...> >(format_string), std::forward<Args>(args)...);
}

template <typename ...Args>
static void LogWarn(std::format_string<Args...> format_string, Args&&... args)
{
    Logger::Log(Logger::warn, std::forward<std::format_string<Args...> >(format_string), std::forward<Args>(args)...);
}

template <typename ...Args>
static void LogError(std::format_string<Args...> format_string, Args&&... args)
{
    Logger::Log(Logger::error, std::forward<std::format_string<Args...> >(format_string), std::forward<Args>(args)...);
}

#endif
