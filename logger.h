#ifndef LOGGER_H
#define LOGGER_H

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
    static void Log(log_levels level, const char* format, ...);

private:
    static constexpr const char* level_names[] {"DBG", "INF", "WRN", "ERR"};
};

#define LOG_DEBUG(format, ...) Logger::Log(Logger::debug, format __VA_OPT__(,) __VA_ARGS__ );
#define LOG_INFO(format, ...) Logger::Log(Logger::info, format __VA_OPT__(,) __VA_ARGS__ );
#define LOG_WARN(format, ...) Logger::Log(Logger::warn, format __VA_OPT__(,) __VA_ARGS__ );
#define LOG_ERROR(format, ...) Logger::Log(Logger::error, format __VA_OPT__(,) __VA_ARGS__ );


#endif
