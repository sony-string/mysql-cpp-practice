#include <map>
#include <string>
#include <memory>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include "../utils.h"
#include "ClubStudentTable.h"

ClubStudentTable::ClubStudentTable(std::shared_ptr<sql::Connection> conn)
    : BasicTable("Club_Student", conn) {}

bool ClubStudentTable::create_club_student(int student_id, int club_id) {
    try {
        std::map<std::string, std::string> attributes;
        attributes["student_id"] = std::to_string(student_id);
        attributes["club_id"] = std::to_string(club_id);

        if (!basic_insert(attributes)) {
            throw std::runtime_error("Failed to create club-student relationship for student ID: " + std::to_string(student_id) + ", club ID: " + std::to_string(club_id));
        }

        Logger(ll_info, "Successfully created club-student relationship: student ID = " + std::to_string(student_id) + ", club ID = " + std::to_string(club_id)).log();
        return true;
    } catch (const std::exception &e) {
        Logger(ll_error, e.what()).log();
        return false;
    }
}

std::unique_ptr<sql::ResultSet> ClubStudentTable::read_by_student_id(int student_id) {
    auto result = basic_select({{"student_id", std::to_string(student_id)}});
    if (!result) {
        Logger(ll_info, "No relationships found for student ID: " + std::to_string(student_id)).log();
    }
    return result;
}

std::unique_ptr<sql::ResultSet> ClubStudentTable::read_by_club_id(int club_id) {
    auto result = basic_select({{"club_id", std::to_string(club_id)}});
    if (!result) {
        Logger(ll_info, "No relationships found for club ID: " + std::to_string(club_id)).log();
    }
    return result;
}

bool ClubStudentTable::delete_club_student(int student_id, int club_id) {
    try {
        std::map<std::string, std::string> conditions = {
            {"student_id", std::to_string(student_id)},
            {"club_id", std::to_string(club_id)}
        };

        if (!basic_delete(conditions)) {
            throw std::runtime_error("Failed to delete club-student relationship for student ID: " + std::to_string(student_id) + ", club ID: " + std::to_string(club_id));
        }

        Logger(ll_info, "Successfully deleted club-student relationship: student ID = " + std::to_string(student_id) + ", club ID = " + std::to_string(club_id)).log();
        return true;
    } catch (const std::exception &e) {
        Logger(ll_error, e.what()).log();
        return false;
    }
}