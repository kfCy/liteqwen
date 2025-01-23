//
// Created by kaifeng yang on 2025/1/23.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <cstdarg>
#include <string>

class Logger {
public:
    // 日志级别枚举
    enum class LogLevel {
        INFO,
        WARN,
        DEBUG,
        ERROR
    };

    // 日志输出接口
    static void log(LogLevel level, const char* format, ...);

    // 各级别日志输出接口
    static void info(const char* format, ...);
    static void warn(const char* format, ...);
    static void debug(const char* format, ...);
    static void error(const char* format, ...);

private:
    // 将日志级别转换为字符串
    static const char* logLevelToString(LogLevel level);
};

#endif // LOGGER_H

