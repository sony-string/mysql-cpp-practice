#include <map>
#include <string>
#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "../utils.h"
#include "BasicTable.h"

BasicTable::BasicTable(std::string name, std::shared_ptr<sql::Connection> conn): table_name(name), con(conn) {
    try {
        std::string query = "DESCRIBE " + table_name;
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());        

        Logger(ll_info, "executeQuery: " + query).log();     
        while (res->next()) {
            std::string field = res->getString("Field");
            columns.push_back(field);
        }                
    } catch (sql::SQLException &e) {
        Logger(ll_error, "Error in BasicTable: " + std::string(e.what())).log();        
    }
}

bool BasicTable::basic_insert(std::map<std::string, std::string> attributes) {
    try {
        std::string columns, values;
        for (const auto &pair : attributes) {
            if (!columns.empty()) {
                columns += ", ";
                values += ", ";
            }
            columns += pair.first;
            values += "'" + pair.second + "'";
        }

        std::string query = "INSERT INTO " + table_name + "(" + columns + ") VALUES (" + values + ")";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->execute();
        Logger(ll_info, "executeQuery: " + query).log();
        return true;
    } catch (sql::SQLException &e) {
        Logger(ll_error, "Error in basic_insert: " + std::string(e.what())).log();
        return false;
    }
}

bool BasicTable::basic_show() {
    try {
        std::string query = "DESCRIBE " + table_name;
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        Logger(ll_info, "executeQuery: " + query).log();

        std::cout << "Table structure for '" << table_name << "':" << std::endl;
        print_result_set(res);
        return true;
    } catch (sql::SQLException &e) {
        Logger(ll_error, "Error in basic_show: " + std::string(e.what())).log();
        return false;
    }
}

bool BasicTable::basic_delete(std::map<std::string, std::string> conditions) {
    try {
        std::string query = "DELETE FROM " + table_name + " WHERE ";
        bool first = true;
        for (const auto &pair : conditions) {
            if (!first) query += " AND ";
            query += pair.first + "='" + pair.second + "'";
            first = false;
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        if (pstmt->executeUpdate() == 1) {
            Logger(ll_info, "executeQuery: " + query).log();
            return true;
        } else {
            Logger(ll_info, "A single matching tuple was not found in basic_delete.").log();
            return false;
        }
    } catch (sql::SQLException &e) {
        Logger(ll_error, "Error in basic_delete: " + std::string(e.what())).log();
        return false;
    }
}

bool BasicTable::basic_update(std::map<std::string, std::string> conditions, std::map<std::string, std::string> new_values) {
    try {
        std::string query = "UPDATE " + table_name + " SET ";
        bool first = true;
        for (const auto &pair : new_values) {
            if (!first) query += ", ";
            query += pair.first + "='" + pair.second + "'";
            first = false;
        }

        query += " WHERE ";
        first = true;
        for (const auto &pair : conditions) {
            if (!first) query += " AND ";
            query += pair.first + "='" + pair.second + "'";
            first = false;
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        if (pstmt->executeUpdate() == 1) {
            Logger(ll_info, "executeQuery: " + query).log();
            return true;
        } else {
            Logger(ll_info, "A single matching tuple was not found in basic_update.").log();
            return false;
        }
    } catch (sql::SQLException &e) {
        Logger(ll_error, "Error in basic_update: " + std::string(e.what())).log();
        return false;
    }
}

std::unique_ptr<sql::ResultSet> BasicTable::basic_string_select(std::map<std::string, std::string> conditions) {    
    try {
        std::string query = "SELECT * FROM " + table_name + " WHERE ";
        bool first = true;
        for (const auto &pair : conditions) {
            if (!first) query += " AND ";
            query += pair.first + " like '%" + pair.second + "%'";
            first = false;
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();

        return res;
    } catch (sql::SQLException &e) {
        Logger(ll_error, "Error in basic_select: " + std::string(e.what())).log();
        return nullptr;
    }
}

std::unique_ptr<sql::ResultSet> BasicTable::basic_select(std::map<std::string, std::string> conditions) {    
    try {
        std::string query = "SELECT * FROM " + table_name + " WHERE ";
        bool first = true;
        for (const auto &pair : conditions) {
            if (!first) query += " AND ";
            query += pair.first + " = '" + pair.second + "'";
            first = false;
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();

        return res;
    } catch (sql::SQLException &e) {
        Logger(ll_error, "Error in basic_select: " + std::string(e.what())).log();
        return nullptr;
    }
}
