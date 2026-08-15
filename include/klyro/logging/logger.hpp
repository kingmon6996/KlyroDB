#ifndef KLYRO_LOGGING_LOGGER_HPP
#define KLYRO_LOGGING_LOGGER_HPP

#include <string_view>
#include <mutex>
#include <iostream>

namespace klyro::logging {

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

// A minimal, thread-safe logging abstraction.
class Logger {
public:
    static Logger& get_instance() {
        static Logger instance;
        return instance;
    }

    void set_level(LogLevel level) noexcept {
        m_level = level;
    }

    bool is_enabled(LogLevel level) const noexcept {
        return level >= m_level;
    }

    void log(LogLevel level, std::string_view component, std::string_view message);

private:
    Logger() = default;

    LogLevel m_level{LogLevel::INFO};
    std::mutex m_mutex;
};

// Macros to avoid expensive message construction if the level is disabled
#define KLYRO_LOG_TRACE(component, message) \
    do { \
        if (klyro::logging::Logger::get_instance().is_enabled(klyro::logging::LogLevel::TRACE)) { \
            klyro::logging::Logger::get_instance().log(klyro::logging::LogLevel::TRACE, component, message); \
        } \
    } while(0)

#define KLYRO_LOG_DEBUG(component, message) \
    do { \
        if (klyro::logging::Logger::get_instance().is_enabled(klyro::logging::LogLevel::DEBUG)) { \
            klyro::logging::Logger::get_instance().log(klyro::logging::LogLevel::DEBUG, component, message); \
        } \
    } while(0)

#define KLYRO_LOG_INFO(component, message) \
    do { \
        if (klyro::logging::Logger::get_instance().is_enabled(klyro::logging::LogLevel::INFO)) { \
            klyro::logging::Logger::get_instance().log(klyro::logging::LogLevel::INFO, component, message); \
        } \
    } while(0)

#define KLYRO_LOG_WARN(component, message) \
    do { \
        if (klyro::logging::Logger::get_instance().is_enabled(klyro::logging::LogLevel::WARN)) { \
            klyro::logging::Logger::get_instance().log(klyro::logging::LogLevel::WARN, component, message); \
        } \
    } while(0)

#define KLYRO_LOG_ERROR(component, message) \
    do { \
        if (klyro::logging::Logger::get_instance().is_enabled(klyro::logging::LogLevel::ERROR)) { \
            klyro::logging::Logger::get_instance().log(klyro::logging::LogLevel::ERROR, component, message); \
        } \
    } while(0)

#define KLYRO_LOG_FATAL(component, message) \
    do { \
        if (klyro::logging::Logger::get_instance().is_enabled(klyro::logging::LogLevel::FATAL)) { \
            klyro::logging::Logger::get_instance().log(klyro::logging::LogLevel::FATAL, component, message); \
        } \
    } while(0)

} // namespace klyro::logging

#endif // KLYRO_LOGGING_LOGGER_HPP
