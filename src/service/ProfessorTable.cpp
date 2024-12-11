#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <map>
#include <memory>
#include <string>

#include "../utils.h"
#include "ProfessorTable.h"

ProfessorTable::ProfessorTable(std::shared_ptr<sql::Connection> conn)
    : BasicTable("Professor", conn) {}

bool ProfessorTable::create_professor(const std::string &name) {
    try {
        std::map<std::string, std::string> attributes;
        attributes["name"] = name;

        if (!basic_insert(attributes)) {
            throw std::runtime_error("Failed to create professor: " + name);
        }

        Logger(ll_info, "Successfully created professor: " + name).log();
        return true;
    } catch (const std::exception &e) {
        Logger(ll_error, e.what()).log();
        return false;
    }
}

std::unique_ptr<sql::ResultSet> ProfessorTable::read_professor_by_id(int prof_id) {
    auto result = basic_select({{"prof_id", std::to_string(prof_id)}});
    if (!result) {
        Logger(ll_info, "Failed to find professor with ID: " + std::to_string(prof_id)).log();
    }
    return result;
}

std::unique_ptr<sql::ResultSet> ProfessorTable::read_professor_by_club_id(int club_id) {
    try {
        std::string query = "SELECT * FROM professor WHERE prof_id IN (SELECT prof_id FROM club WHERE club_id = ?)";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, club_id);

        std::unique_ptr<sql::ResultSet> result(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();

        if (!result || result->rowsCount() == 0) {
            Logger(ll_info, "No professors found for club with ID: " + std::to_string(club_id)).log();
        }

        return result;
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "SQL error in read_professor_by_club_id: " + std::string(e.what())).log();
        return nullptr;
    }
}

bool ProfessorTable::update_professor_name(int prof_id, const std::string &new_name) {
    try {
        std::map<std::string, std::string> conditions = {{"prof_id", std::to_string(prof_id)}};
        std::map<std::string, std::string> new_values = {{"name", new_name}};

        if (!basic_update(conditions, new_values)) {
            throw std::runtime_error("Failed to update professor's name for ID: " + std::to_string(prof_id));
        }

        Logger(ll_info, "Successfully updated professor's name for ID: " + std::to_string(prof_id)).log();
        return true;
    } catch (const std::exception &e) {
        Logger(ll_error, e.what()).log();
        return false;
    }
}

bool ProfessorTable::delete_professor(int prof_id) {
    try {
        std::map<std::string, std::string> conditions = {{"prof_id", std::to_string(prof_id)}};

        if (!basic_delete(conditions)) {
            throw std::runtime_error("Failed to delete professor with ID: " + std::to_string(prof_id));
        }

        Logger(ll_info, "Successfully deleted professor with ID: " + std::to_string(prof_id)).log();
        return true;
    } catch (const std::exception &e) {
        Logger(ll_error, e.what()).log();
        return false;
    }
}

std::unique_ptr<sql::ResultSet> ProfessorTable::read_all_professor() {
    return basic_select_all();
}