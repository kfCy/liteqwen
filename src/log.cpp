//
// Created by kaifeng yang on 2025/1/23.
//

#include "log.h"
#include <iostream>
#include <iomanip>  // 引入这个头文件以使用 std::setw 和 std::setfill
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <sstream>
#include <chrono>   // 用于获取毫秒

namespace liteqwen {

    // 获取当前时间的字符串表示
    std::string Logger::getCurrentTime() {
        // 获取当前时间
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);

        // 获取当前时间的tm结构
        std::tm* tm_now = std::localtime(&now_time);

        // 获取当前毫秒数
        auto duration = now.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;

        std::ostringstream oss;

        // 格式化日期和时间，保证月份、日期、小时、分钟、秒都是两位
        oss << std::setw(2) << std::setfill('0') << (tm_now->tm_mon + 1) << "/"   // 月份
            << std::setw(2) << std::setfill('0') << tm_now->tm_mday << "/"          // 日期
            << (tm_now->tm_year + 1900) << " "                                      // 年份
            << std::setw(2) << std::setfill('0') << tm_now->tm_hour << ":"         // 小时
            << std::setw(2) << std::setfill('0') << tm_now->tm_min << ":"          // 分钟
            << std::setw(2) << std::setfill('0') << tm_now->tm_sec << "."          // 秒钟
            << std::setw(3) << std::setfill('0') << milliseconds;                   // 毫秒数

        return oss.str();
    }

    // 将日志级别转换为对应的字符串
    const char* Logger::logLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::INFO:
                return "INFO";
            case LogLevel::WARN:
                return "WARN";
            case LogLevel::DEBUG:
                return "DEBUG";
            case LogLevel::ERROR:
                return "ERROR";
            default:
                return "UNKNOWN";
        }
    }

    // 格式化并打印日志
    void Logger::log(LogLevel level, const char* format, va_list args) {
        const char* levelStr = logLevelToString(level);
        std::string currentTime = getCurrentTime();

        // 打印日期时间和日志级别
        printf("%s - %s - ", currentTime.c_str(), levelStr);

        // 可变参数处理
        vprintf(format, args);  // 使用 vprintf 打印日志
    }

    // 输出INFO级别的日志
    void Logger::info(const char* format, ...) {
        va_list args;
        va_start(args, format); // 可变参数：va_start和va_end必须成对出现
        log(LogLevel::INFO, format, args);
        va_end(args);
    }

    // 输出WARN级别的日志
    void Logger::warn(const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LogLevel::WARN, format, args);
        va_end(args);
    }

    // 输出DEBUG级别的日志
    void Logger::debug(const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LogLevel::DEBUG, format, args);
        va_end(args);
    }

    // 输出ERROR级别的日志
    void Logger::error(const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LogLevel::ERROR, format, args);
        va_end(args);
    }

}
//
//int main() {
//    //setting up inference on data_id=0, using devices=[0, 1), num_layers=48, max_dynamic_bsz=16, max_BL=5120
//    int data_id = 0;
//    int input_device = 0;
//    int output_device = 0;
//    int num_layer = 48;
//    int max_dynamic_bsz = 16;
//    int max_BL = 5120;
//    printf("setting up inference on data_id=%i, using devices=[%i, %i), num_layers=%i, max_dynamic_bsz=%i, max_BL=%i\n",
//           data_id, input_device, output_device+1, num_layer, max_dynamic_bsz, max_BL);
//    liteqwen::Logger::info("setting up inference on data_id=%i, using devices=[%i, %i), num_layers=%i, max_dynamic_bsz=%i, max_BL=%i\n",
//                 data_id, input_device, output_device+1, num_layer, max_dynamic_bsz, max_BL);
//    liteqwen::Logger::info("This is an info log.");
//    liteqwen::Logger::warn("This is a warning log.");
//    liteqwen::Logger::debug("This is a debug log.");
//    liteqwen::Logger::error("This is an error log.");
//    return 0;
//}
