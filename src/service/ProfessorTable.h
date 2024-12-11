#pragma once

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <map>
#include <memory>
#include <string>

#include "../utils.h"
#include "BasicTable.h"

/**
 * @brief Represents the professor table with specific CRUD operations.
 */
class ProfessorTable : public BasicTable {
public:
    /**
     * @brief Constructs a new ProfessorTable object with a database connection.
     * @param conn A shared pointer to an active database connection.
     */
    ProfessorTable(std::shared_ptr<sql::Connection> conn);

    /**
     * @brief Creates a new professor record.
     * @param name The name of the professor.
     * @return True if the professor was created successfully, false otherwise.
     */
    bool create_professor(const std::string &name);

    /**
     * @brief Reads a professor record based on professor ID.
     * @param prof_id The ID of the professor.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_professor_by_id(int prof_id);

    /**
     * @brief Reads professors advising specific clubs.
     * @param club_id The ID of the club.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_professor_by_club_id(int club_id);

    /**
     * @brief Updates the name of a professor.
     * @param prof_id The ID of the professor to update.
     * @param new_name The new name for the professor.
     * @return True if the professor's name was updated successfully, false otherwise.
     */
    bool update_professor_name(int prof_id, const std::string &new_name);

    /**
     * @brief Deletes a professor record.
     * @param prof_id The ID of the professor to delete.
     * @return True if the professor was deleted successfully, false otherwise.
     */
    bool delete_professor(int prof_id);

    /**
     * @brief Reads all professors in the table.
     * @return A unique pointer to a ResultSet containing all clubs, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_all_professor();
};