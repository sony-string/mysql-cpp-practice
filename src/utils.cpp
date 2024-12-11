#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <limits>

#include <cppconn/resultset.h>

#include "utils.h"

constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

void clear_cin_error() {
    std::cin.clear();            
    std::cin.ignore(max_size, '\n');
}

void clear_cin_buffer() {
    std::cin.ignore(max_size, '\n');
}

void print_separator(const std::vector<int>& column_widths) {
    for (const auto& width : column_widths) {
        std::cout << "+" << std::string(width + 2, '-');
    }
    std::cout << "+\n";
}

void print_row(const std::vector<std::string>& values, const std::vector<int>& column_widths) {
    for (size_t i = 0; i < values.size(); ++i) {
        std::cout << "| " << std::setw(column_widths[i]) << std::left << values[i] << " ";
    }
    std::cout << "|\n";
}

void print_result_set(std::unique_ptr<sql::ResultSet>& res) {
    if (res == nullptr) {
        Logger(ll_error, "ResultSet is null");
        return;
    }

    // Retrieve metadata
    sql::ResultSetMetaData* metadata = res->getMetaData();
    int column_count = metadata->getColumnCount();    

    std::vector<std::string> column_names(column_count);
    std::vector<int> column_widths(column_count, 10);
    
    // Initialize column names and determine maximum widths
    for (int i = 1; i <= column_count; ++i) {
        std::string column_name = metadata->getColumnName(i);
        column_names[i - 1] = column_name;
        column_widths[i - 1] = std::max(column_widths[i - 1], static_cast<int>(column_name.length()));
    }

    std::vector<std::vector<std::string>> rows;

    // Store each row
    while (res->next()) {
        std::vector<std::string> row_values(column_count);
        for (int i = 1; i <= column_count; ++i) {
            std::string value = res->getString(i);
            row_values[i - 1] = value;
            column_widths[i - 1] = std::max(column_widths[i - 1], static_cast<int>(value.length()));
        }
        rows.push_back(row_values);
    }

    // Print table
    print_separator(column_widths);
    print_row(column_names, column_widths);
    print_separator(column_widths);
    for (const auto& row : rows) {
        print_row(row, column_widths);
    }
    print_separator(column_widths);
}