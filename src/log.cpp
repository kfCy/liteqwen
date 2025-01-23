//
// Created by kaifeng yang on 2025/1/23.
//

#include "log.h"
#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdarg>

namespace liteqwen {

    void Logger::log(LogLevel level, const char* format, ...) {
        // 获取当前时间
        std::time_t now = std::time(nullptr);
        std::tm* tm_now = std::localtime(&now);

        // 格式化时间戳
        char timestamp[20];
        std::strftime(timestamp, sizeof(timestamp), "%m/%d/%Y %H:%M:%S", tm_now);

        // 获取日志级别的字符串表示
        const char* level_str = logLevelToString(level);

        // 打印时间戳和日志级别
        printf("%s %s ", timestamp, level_str);

        // 打印具体的日志内容
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        // 打印完日志后换行
        printf("\n");
    }

    void Logger::info(const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LogLevel::INFO, format, args);
        va_end(args);
    }

    void Logger::warn(const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LogLevel::WARN, format, args);
        va_end(args);
    }

    void Logger::debug(const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LogLevel::DEBUG, format, args);
        va_end(args);
    }

    void Logger::error(const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LogLevel::ERROR, format, args);
        va_end(args);
    }

    const char* Logger::logLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARN: return "WARN";
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

}

//int main() {
//    Logger::info("This is an info log.");
//    Logger::warn("This is a warning log.");
//    Logger::debug("This is a debug log.");
//    Logger::error("This is an error log.");
//    return 0;
//}
