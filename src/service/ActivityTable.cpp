#include <memory>
#include <map>
#include <string>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>

#include "../utils.h"
#include "BasicTable.h"
#include "ActivityTable.h"

ActivityTable::ActivityTable(std::shared_ptr<sql::Connection> conn) : BasicTable("Activity", conn) {}

bool ActivityTable::create_activity(int club_id, const std::string& act_title, 
                                    const std::string& start_date, const std::string& end_date) {
    try {
        std::string query = "INSERT INTO Activity (club_id, act_title, start_date, end_date) VALUES (?, ?, ";

        if (start_date.empty()) {
            query += "NOW(), ?";
        } else {
            query += "?, ?";
        }
        query += ")";

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, club_id);
        pstmt->setString(2, act_title);
        if (!start_date.empty()) {
            pstmt->setString(3, start_date);
            pstmt->setString(4, end_date);
        } else {
            pstmt->setString(3, end_date);
        }
        pstmt->executeUpdate();        
        Logger(ll_info, "executeQuery: " + query).log();
    } catch (const sql::SQLException& e) {
        Logger(ll_error, "Error in create_activity: " + std::string(e.what())).log();
        return false;
    }
    return true;
}

std::unique_ptr<sql::ResultSet> ActivityTable::read_activity_by_club_id(int club_id) {
    try {
        std::string query = "SELECT * FROM Activity WHERE club_id = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, club_id);        
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();
        return res;
    } catch (const sql::SQLException& e) {
        Logger(ll_error, "Error in read_activity_by_club_id: " + std::string(e.what())).log();
        return nullptr;
    }
}

std::unique_ptr<sql::ResultSet> ActivityTable::read_activity_by_title(const std::string& act_title, int club_id) {
    try {        
        std::string query = "SELECT * FROM Activity WHERE act_title LIKE ?";
        if (club_id != -1)
            query += " AND club_id = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setString(1, '%' + act_title + '%');
        if (club_id != -1)
            pstmt->setInt(2, club_id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();
        return res;
    } catch (const sql::SQLException& e) {
        Logger(ll_error, "Error in read_activity_by_title: " + std::string(e.what())).log();
        return nullptr;
    }
}

std::unique_ptr<sql::ResultSet> ActivityTable::read_activity_by_period(const std::string& from_date, const std::string& to_date, int club_id) {
    try {
        std::string query = "SELECT * FROM Activity WHERE (start_date <= ? AND (end_date >= ? OR end_date IS NULL))";
        if (club_id != -1)
            query += " AND club_id = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setString(1, to_date);
        pstmt->setString(2, from_date);
        if (club_id != -1)
            pstmt->setInt(3, club_id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();
        return res;
    } catch (const sql::SQLException& e) {
        Logger(ll_error, "Error in read_activity_by_period: " + std::string(e.what())).log();
        return nullptr;
    }
}

std::unique_ptr<sql::ResultSet> ActivityTable::read_activity_by_id(int act_id) {
    try {
        std::string query = "SELECT * FROM Activity WHERE act_id = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, act_id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();
        return res;
    } catch (const sql::SQLException& e) {
        Logger(ll_error, "Error in read_activity_by_id: " + std::string(e.what())).log();
        return nullptr;
    }
}

bool ActivityTable::update_activity(int act_id, const std::map<std::string, std::string>& updates) {
    try {
        std::string query = "UPDATE Activity SET ";
        for (auto it = updates.begin(); it != updates.end(); ++it) {
            query += it->first + " = ?";
            if (std::next(it) != updates.end()) {
                query += ", ";
            }
        }
        query += " WHERE act_id = ?";

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        int param_index = 1;
        for (const auto& kv : updates) {
            pstmt->setString(param_index++, kv.second);
        }
        pstmt->setInt(param_index, act_id);
        pstmt->executeUpdate();
        Logger(ll_info, "executeQuery: " + query).log();
    } catch (const sql::SQLException& e) {
        Logger(ll_error, "Error in update_activity: " + std::string(e.what())).log();
        return false;
    }
    return true;
}

bool ActivityTable::delete_activity(int act_id) {
    try {
        std::string query = "DELETE FROM Activity WHERE act_id = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, act_id);
        pstmt->executeUpdate();
        Logger(ll_info, "executeQuery: " + query).log();
    } catch (const sql::SQLException& e) {
        Logger(ll_error, "Error in delete_activity: " + std::string(e.what())).log();
        return false;
    }
    return true;
}