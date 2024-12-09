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

static std::string get_loglevel_string(loglevel level) {
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

/**
 * @brief Helper function to print a separator line for the table.
 * @param column_widths Vector of column widths for formatting.
 */
void print_separator(const std::vector<int>& column_widths);

/**
 * @brief Helper function to print a row with column values in the table.
 * @param values Vector of string values for the row.
 * @param column_widths Vector of column widths for formatting.
 */
void print_row(const std::vector<std::string>& values, const std::vector<int>& column_widths);

/**
 * @brief Prints the ResultSet as a table to the console.
 * @param res A unique pointer to the ResultSet containing the query results.
 */
void print_result_set(std::unique_ptr<sql::ResultSet>& res);