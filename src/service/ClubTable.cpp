
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <map>
#include <memory>
#include <string>

#include "../utils.h"
#include "ClubTable.h"

ClubTable::ClubTable(std::shared_ptr<sql::Connection> conn)
        : BasicTable("Club", conn) {}

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
        std::string query = "SELECT * FROM clubs WHERE club_id IN (SELECT club_id FROM Location WHERE loc_id = ?)";
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
        std::string query = "SELECT * FROM clubs WHERE club_id IN (SELECT club_id FROM Location WHERE loc_name like '%?%')";
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
