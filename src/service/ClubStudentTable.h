#pragma once

#include <map>
#include <string>
#include <memory>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include "../utils.h"
#include "BasicTable.h"

/**
 * @brief Represents the club-student relationship table with specific CRUD operations.
 */
class ClubStudentTable : public BasicTable {
public:
    /**
     * @brief Constructs a new ClubStudentTable object with a database connection.
     * @param conn A shared pointer to an active database connection.
     */
    ClubStudentTable(std::shared_ptr<sql::Connection> conn);

    /**
     * @brief Creates a new club-student relationship record.
     * For this table, the default relationship can be created by direct assignment without input.
     * @param student_id The ID of the student.
     * @param club_id The ID of the club.
     * @return True if the relationship was created successfully, false otherwise.
     */
    bool create_club_student(int student_id, int club_id);

    /**
     * @brief Reads relationships based on student ID.
     * @param student_id The ID of the student.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_by_student_id(int student_id);

    /**
     * @brief Reads relationships based on club ID.
     * @param club_id The ID of the club.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_by_club_id(int club_id);

    /**
     * @brief Deletes a club-student relationship.
     * @param student_id The ID of the student.
     * @param club_id The ID of the club.
     * @return True if the relationship was deleted successfully, false otherwise.
     */
    bool delete_club_student(int student_id, int club_id);
};