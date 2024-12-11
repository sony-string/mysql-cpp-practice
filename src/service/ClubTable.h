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
 * @brief Represents the club table with specific CRUD operations.
 */
class ClubTable : public BasicTable {
public:
    /**
     * @brief Constructs a new ClubTable object with a database connection.
     * @param conn A shared pointer to an active database connection.
     */
    ClubTable(std::shared_ptr<sql::Connection> conn);

    /**
     * @brief Creates a new club record.
     * @param club_name The unique name of the club.
     * @param budget The budget for the club.
     * @param prof_id The ID of the advising professor.
     * @return True if the club was created successfully, false otherwise.
     */
    bool create_club(const std::string &club_name, double budget, int prof_id);

    /**
     * @brief Reads a club record based on club ID.
     * @param club_id The ID of the club.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_club_by_id(int club_id);

    /**
     * @brief Reads a club record based on club name.
     * @param club_name The name of the club.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_club_by_name(const std::string &club_name);

    /**
     * @brief Reads clubs using a specific location ID.
     * @param loc_id The ID of the location.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_club_by_location_id(int loc_id);

    /**
     * @brief Reads clubs using a specific location name.
     * @param loc_name The name of the location.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_club_by_location_name(const std::string &loc_name);

    /**
     * @brief Reads clubs advised by a specific professor.
     * @param prof_id The ID of the professor.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_club_by_prof_id(int prof_id);

    /**
     * @brief Updates the name of a club.
     * @param club_id The ID of the club to update.
     * @param new_name The new name for the club.
     * @return True if the club's name was updated successfully, false otherwise.
     */
    bool update_club_name(int club_id, const std::string &new_name);

    /**
     * @brief Updates the budget of a club.
     * @param club_id The ID of the club to update.
     * @param new_budget The new budget for the club.
     * @return True if the club's budget was updated successfully, false otherwise.
     */
    bool update_club_budget(int club_id, double new_budget);

    /**
     * @brief Updates the professor advising a club.
     * @param club_id The ID of the club to update.
     * @param new_prof_id The new professor ID.
     * @return True if the club's advising professor was updated successfully, false otherwise.
     */
    bool update_club_prof_id(int club_id, int new_prof_id);

    /**
     * @brief Deletes a club record.
     * @param club_id The ID of the club to delete.
     * @return True if the club was deleted successfully, false otherwise.
     */
    bool delete_club(int club_id);

    /**
     * @brief Reads all clubs in the table.
     * @return A unique pointer to a ResultSet containing all clubs, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_all_club();
};