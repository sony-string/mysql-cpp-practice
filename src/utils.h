#pragma once

#include <ctime>
#include <iostream>
#include <string>

enum loglevel {
    /**
     * @brief Trace
     */
    ll_trace = 0,

    /**
     * @brief Debug
     */
    ll_debug,

    /**
     * @brief Information
     */
    ll_info,

    /**
     * @brief Warning
     */
    ll_warning,

    /**
     * @brief Error
     */
    ll_error,

    /**
     * @brief Critical
     */
    ll_critical
};

std::string get_loglevel_string(loglevel level) {
    switch (level) {
    case ll_trace:
        return "TRACE";
    case ll_debug:
        return "DEBUG";
    case ll_info:
        return "INFO";
    case ll_warning:
        return "WARNING";
    case ll_error:
        return "ERROR";
    case ll_critical:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}

/**
 * @brief Log messages
 */
class Logger {
private:
    /**
     * @brief Severity.
     */
    loglevel severity = ll_info;

    /**
     * @brief Log Message
     */
    std::string message = {};

public:
    Logger(enum loglevel ll, std::string msg) : severity(ll), message(msg) {}

    void log() {
        std::time_t now = std::time(nullptr);
        std::tm *localTime = std::localtime(&now);

        char timeBuffer[20];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", localTime);

        std::cout << "[" << timeBuffer << "] [" << get_loglevel_string(severity) << "] " << message << '\n';
    }
};

/**
 * @brief Recover error of std::cin error.
 */
void clear_cin_error();