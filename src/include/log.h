#ifndef LOG_H
#define LOG_H

#include <cstdarg>
#include <string>

namespace liteqwen {

    class Logger {
    public:
        // 日志级别枚举
        enum class LogLevel {
            INFO,
            WARN,
            DEBUG,
            ERROR
        };

        /**
         * @brief 格式化并打印日志，输出格式为: 日期 时间 日志级别 消息内容
         *
         * 该函数根据日志级别输出相应的日志信息，并将日志内容格式化为统一的输出格式。
         *
         * 输出示例：
         * - "01/05/2025 23:45:44 INFO This is a general log message"
         *
         * @param level 日志级别 (INFO, WARN, DEBUG, ERROR)
         * @param format 格式化字符串，类似printf中的格式化方式
         * @param ... 格式化字符串的参数
         */
        static void log(LogLevel level, const char* format, ...);

        /**
         * @brief 输出INFO级别的日志
         *
         * 该函数输出INFO级别的日志信息，格式化为统一的输出格式。
         *
         * 输出示例：
         * - "01/05/2025 23:45:44 INFO This is an info log."
         *
         * @param format 格式化字符串，类似printf中的格式化方式
         * @param ... 格式化字符串的参数
         */
        static void info(const char* format, ...);

        /**
         * @brief 输出WARN级别的日志
         *
         * 该函数输出WARN级别的日志信息，格式化为统一的输出格式。
         *
         * 输出示例：
         * - "01/05/2025 23:45:44 WARN This is a warning log."
         *
         * @param format 格式化字符串，类似printf中的格式化方式
         * @param ... 格式化字符串的参数
         */
        static void warn(const char* format, ...);

        /**
         * @brief 输出DEBUG级别的日志
         *
         * 该函数输出DEBUG级别的日志信息，格式化为统一的输出格式。
         *
         * 输出示例：
         * - "01/05/2025 23:45:44 DEBUG This is a debug log."
         *
         * @param format 格式化字符串，类似printf中的格式化方式
         * @param ... 格式化字符串的参数
         */
        static void debug(const char* format, ...);

        /**
         * @brief 输出ERROR级别的日志
         *
         * 该函数输出ERROR级别的日志信息，格式化为统一的输出格式。
         *
         * 输出示例：
         * - "01/05/2025 23:45:44 ERROR This is an error log."
         *
         * @param format 格式化字符串，类似printf中的格式化方式
         * @param ... 格式化字符串的参数
         */
        static void error(const char* format, ...);

    private:
        /**
         * @brief 将日志级别转换为对应的字符串表示
         *
         * 该函数将日志级别 (LogLevel) 转换为对应的字符串表示，以便输出到日志中。
         *
         * @param level 日志级别 (INFO, WARN, DEBUG, ERROR)
         * @return 对应的日志级别字符串
         */
        static const char* logLevelToString(LogLevel level);
    };

} // namespace lite_kk

#endif // LOG_H
