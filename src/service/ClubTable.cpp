#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <map>
#include <memory>
#include <sstream>
#include <string>

#include "../utils.h"
#include "ClubTable.h"

ClubTable::ClubTable(std::shared_ptr<sql::Connection> conn)
    : BasicTable("Club", conn), club_student_table(conn), activity_table(conn) {}

bool ClubTable::create_club(const std::string &club_name, double budget, int prof_id) {
    std::map<std::string, std::string> attributes;
    attributes["club_name"] = club_name;
    attributes["budget"] = std::to_string(budget);
    attributes["prof_id"] = std::to_string(prof_id);

    if (!basic_insert(attributes)) {
        Logger(ll_info, "Failed to create club: " + club_name).log();
        return false;
    }
    return true;
}

std::unique_ptr<sql::ResultSet> ClubTable::read_club_by_id(int club_id) {
    return basic_select({{"club_id", std::to_string(club_id)}});
}

std::unique_ptr<sql::ResultSet> ClubTable::read_club_by_name(const std::string &club_name) {
    return basic_string_select({{"club_name", club_name}});
}

std::unique_ptr<sql::ResultSet> ClubTable::read_club_by_location_id(int loc_id) {
    try {
        std::string query = "SELECT * FROM Club WHERE club_id IN (SELECT club_id FROM Location WHERE loc_id = ?)";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, loc_id);

        std::unique_ptr<sql::ResultSet> result(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();

        if (!result || result->rowsCount() == 0) {
            Logger(ll_info, "No club found with location ID: " + std::to_string(loc_id)).log();
        }

        return result;
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "SQL error in read_club_by_location_id: " + std::string(e.what())).log();
        return nullptr;
    }
}

std::unique_ptr<sql::ResultSet> ClubTable::read_club_by_location_name(const std::string &loc_name) {
    try {
        std::string query = "SELECT * FROM Club WHERE club_id IN (SELECT club_id FROM Location WHERE loc_name like '%?%')";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setString(1, loc_name);

        std::unique_ptr<sql::ResultSet> result(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();

        if (!result || result->rowsCount() == 0) {
            Logger(ll_info, "No club found with location name: " + loc_name).log();
        }

        return result;
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "SQL error in read_club_by_location_name: " + std::string(e.what())).log();
        return nullptr;
    }
}

std::unique_ptr<sql::ResultSet> ClubTable::read_club_by_prof_id(int prof_id) {
    return basic_select({{"prof_id", std::to_string(prof_id)}});
}

std::unique_ptr<sql::ResultSet> ClubTable::read_info(int club_id, std::set<std::string> join_table) {
    try {
        std::ostringstream query;

        // Base query to select club info
        query << "SELECT c.*, ";

        // Create a join clause
        for (const auto &table : join_table) {
            query << "JOIN " << table << " ON c.club_id = " << table << ".club_id ";
        }

        // Build the underlying SQL query
        query << "FROM " << table_name << " AS c WHERE c.club_id = ?";

        std::string query_str = query.str();
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query_str));
        pstmt->setInt(1, club_id);

        std::unique_ptr<sql::ResultSet> result(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query_str).log();

        if (!result || result->rowsCount() == 0) {
            Logger(ll_info, "No information found for club ID: " + std::to_string(club_id)).log();
        }

        return result;
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "SQL error in show_info: " + std::string(e.what())).log();
        return nullptr;
    } catch (const std::exception &e) {
        Logger(ll_error, "Error in show_info: " + std::string(e.what())).log();
        return nullptr;
    }
}

std::unique_ptr<sql::ResultSet> ClubTable::read_members_by_club_id(int club_id) {
    try {
        std::string query = "SELECT * FROM Student WHERE student_id IN (SELECT student_id FROM Club_Student WHERE club_id = ?)";

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, club_id);

        std::unique_ptr<sql::ResultSet> result(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();

        if (!result || result->rowsCount() == 0) {
            Logger(ll_info, "No students found for club ID: " + std::to_string(club_id)).log();
        }

        return result;
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "SQL error in read_students_by_club_id: " + std::string(e.what())).log();
        return nullptr;
    } catch (const std::exception &e) {
        Logger(ll_error, "Error in read_students_by_club_id: " + std::string(e.what())).log();
        return nullptr;
    }
}

std::unique_ptr<sql::ResultSet> ClubTable::read_members_by_name_in_club(int club_id, const std::string &student_name) {
    try {
        std::string query = "SELECT s.* FROM Student AS s "
                            "JOIN Club_Student AS cs ON s.student_id = cs.student_id "
                            "WHERE cs.club_id = ? AND s.name LIKE ?";

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, club_id);
        pstmt->setString(2, '%' + student_name + '%');

        std::unique_ptr<sql::ResultSet> result(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();

        if (!result || result->rowsCount() == 0) {
            Logger(ll_info, "No students found with the name pattern '" + student_name + "' in club ID: " + std::to_string(club_id)).log();
        }

        return result;
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "SQL error in search_students_by_name_in_club: " + std::string(e.what())).log();
        return nullptr;
    } catch (const std::exception &e) {
        Logger(ll_error, "Error in search_students_by_name_in_club: " + std::string(e.what())).log();
        return nullptr;
    }
}

bool ClubTable::update_club_name(int club_id, const std::string &new_name) {
    std::map<std::string, std::string> conditions = {{"club_id", std::to_string(club_id)}};
    std::map<std::string, std::string> new_values = {{"club_name", new_name}};
    if (!basic_update(conditions, new_values)) {
        Logger(ll_info, "Failed to update club name for ID: " + std::to_string(club_id)).log();
        return false;
    }
    return true;
}

bool ClubTable::update_club_budget(int club_id, double new_budget) {
    std::map<std::string, std::string> conditions = {{"club_id", std::to_string(club_id)}};
    std::map<std::string, std::string> new_values = {{"budget", std::to_string(new_budget)}};
    if (!basic_update(conditions, new_values)) {
        Logger(ll_info, "Failed to update club budget for ID: " + std::to_string(club_id)).log();
        return false;
    }
    return true;
}

bool ClubTable::update_club_prof_id(int club_id, int new_prof_id) {
    std::map<std::string, std::string> conditions = {{"club_id", std::to_string(club_id)}};
    std::map<std::string, std::string> new_values = {{"prof_id", std::to_string(new_prof_id)}};
    if (!basic_update(conditions, new_values)) {
        Logger(ll_info, "Failed to update club prof_id for ID: " + std::to_string(club_id)).log();
        return false;
    }
    return true;
}

bool ClubTable::add_member(int club_id, int student_id) {
    return club_student_table.create_club_student(student_id, club_id);
}

bool ClubTable::delete_member(int club_id, int student_id) {
    return club_student_table.delete_club_student(student_id, club_id);
}

bool ClubTable::delete_club(int club_id) {
    std::map<std::string, std::string> conditions = {{"club_id", std::to_string(club_id)}};
    if (!basic_delete(conditions)) {
        Logger(ll_info, "Failed to delete club with ID: " + std::to_string(club_id)).log();
        return false;
    }
    return true;
}

std::unique_ptr<sql::ResultSet> ClubTable::read_all_club() {
    return basic_select_all();
}

// Activities

bool ClubTable::create_activity_for_club(int club_id, const std::string &act_title, const std::string &start_date, const std::string &end_date) {
    if (end_date.empty())
        return activity_table.create_activity(club_id, act_title, start_date);
    return activity_table.create_activity(club_id, act_title, start_date, end_date);
}

std::unique_ptr<sql::ResultSet> ClubTable::read_activities_by_club(int club_id) {
    return activity_table.read_activity_by_club_id(club_id);
}

std::unique_ptr<sql::ResultSet> ClubTable::read_activity_by_id(int club_id, int act_id) {
    if (!validate_activity_belongs_to_club(club_id, act_id)) {
        Logger(ll_info, "Activity does not belong to the given club").log();
        return nullptr;
    }
    return activity_table.read_activity_by_id(act_id);
}

bool ClubTable::update_activity_for_club(int club_id, int act_id, const std::map<std::string, std::string> &updates) {
    if (!validate_activity_belongs_to_club(club_id, act_id)) {
        Logger(ll_info, "Activity does not belong to the given club").log();
        return false;
    }
    return activity_table.update_activity(act_id, updates);
}

bool ClubTable::delete_activity_for_club(int club_id, int act_id) {
    if (!validate_activity_belongs_to_club(club_id, act_id)) {
        Logger(ll_info, "Activity does not belong to the given club").log();
        return false;
    }
    return activity_table.delete_activity(act_id);
}

bool ClubTable::validate_activity_belongs_to_club(int club_id, int act_id) {
    auto result = activity_table.read_activity_by_id(act_id);
    if (result && result->next()) {
        return result->getInt("club_id") == club_id;
    }
    return false;
}

std::unique_ptr<sql::ResultSet> ClubTable::read_activity_by_title(int club_id, const std::string &act_title) {
    return activity_table.read_activity_by_title(act_title, club_id);
}

std::unique_ptr<sql::ResultSet> ClubTable::read_activity_by_period(int club_id, const std::string &from_date, const std::string &to_date) {
    return activity_table.read_activity_by_period(from_date, to_date, club_id);
}