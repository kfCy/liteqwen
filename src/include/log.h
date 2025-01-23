#ifndef LOG_H
#define LOG_H

#include <cstdarg>
#include <string>

namespace liteqwen {

    class Logger {
    public:
        enum class LogLevel {
            INFO,
            WARN,
            DEBUG,
            ERROR
        };

        /**
         * @brief 输出INFO级别的日志
         *
         * @param format 格式化字符串，类似printf中的格式化方式
         * @param ... 格式化字符串的参数
         */
        static void info(const char* format, ...);

        /**
         * @brief 输出WARN级别的日志
         *
         * @param format 格式化字符串，类似printf中的格式化方式
         * @param ... 格式化字符串的参数
         */
        static void warn(const char* format, ...);

        /**
         * @brief 输出DEBUG级别的日志
         *
         * @param format 格式化字符串，类似printf中的格式化方式
         * @param ... 格式化字符串的参数
         */
        static void debug(const char* format, ...);

        /**
         * @brief 输出ERROR级别的日志
         *
         * @param format 格式化字符串，类似printf中的格式化方式
         * @param ... 格式化字符串的参数
         */
        static void error(const char* format, ...);

    private:
        /**
         * @brief 格式化并打印日志，输出格式为: 日期 时间 日志级别 消息内容
         *
         * @param level 日志级别 (INFO, WARN, DEBUG, ERROR)
         * @param format 格式化字符串，类似printf中的格式化方式
         * @param ... 格式化字符串的参数
         */
        static void log(LogLevel level, const char* format, va_list args);

        /**
         * @brief 将日志级别转换为对应的字符串表示
         *
         * @param level 日志级别 (INFO, WARN, DEBUG, ERROR)
         * @return 对应的日志级别字符串
         */
        static const char* logLevelToString(LogLevel level);

        /**
         * @brief 获取当前时间的字符串表示
         *
         * @return 当前时间的字符串表示，例如 "01/05/2025 23:45:44"
         */
        static std::string getCurrentTime();
    };

} // namespace lite_kk

#endif // LOG_H
