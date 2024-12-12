#pragma once

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "../utils.h"
#include "ActivityTable.h"
#include "BasicTable.h"
#include "ClubStudentTable.h"

/**
 * @brief Represents the club table with specific CRUD operations.
 */
class ClubTable : public BasicTable {
  protected:
    ClubStudentTable club_student_table;
    ActivityTable activity_table;

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
     * @brief Reads info of a club with joining given tables.
     * @param club_id The ID of the club to see.
     * @param join_table The names of tables to join for presentation.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_info(int club_id, std::set<std::string> join_table);

    /**
     * @brief Reads list of students belong to such club. In short, reading member of the club.
     * @param club_id The ID of the club that students belong to.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_members_by_club_id(int club_id);

    /**
     * @brief Reads students in the specified club which have names matching the given pattern.
     * @param club_id The ID of the club to search for students.
     * @param name_pattern The name pattern to match; can include '%' as a wildcard for LIKE queries.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     */
    std::unique_ptr<sql::ResultSet> read_members_by_name_in_club(int club_id, const std::string &student_name);

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
     * @brief Adds a member to a club.
     * @param club_id The ID of the club to be added.
     * @param student_id The ID of the student to add to club.
     * @return True if the adding a member of club successfully, false otherwise.
     */
    bool add_member(int club_id, int student_id);

    /**
     * @brief Deletes a member from a club.
     * @param club_id The ID of the club to be added.
     * @param student_id The ID of the student to add to club.
     * @return True if the deleting a member of club successfully, false otherwise.
     */
    bool delete_member(int club_id, int student_id);

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

    /**
     * @brief Validates that a given activity belongs to a specific club.
     *
     * @param club_id The ID of the club to which the activity must belong.
     * @param act_id The ID of the activity whose membership is to be validated.
     * @return true If the activity belongs to the club.
     * @return false If the activity does not belong to the club.
     */
    bool validate_activity_belongs_to_club(int club_id, int act_id);

    /**
     * @brief Creates a new activity for a specific club.
     * @param club_id The ID of the club for which the activity is being created.
     * @param act_title The title of the activity.
     * @param start_date The start date of the activity (optional, defaults to current date).
     * @param end_date The end date of the activity (optional, defaults to "2099-12-31").
     * @return True If the activity was successfully created, false otherwise.
     */
    bool create_activity_for_club(int club_id, const std::string &act_title,
                                  const std::string &start_date = "", const std::string &end_date = "2099-12-31");

    /**
     * @brief Retrieves activities associated with a particular club.
     * @param club_id The ID of the club whose activities are to be retrieved.
     * @return std::unique_ptr<sql::ResultSet> Result set containing the activities.
     */
    std::unique_ptr<sql::ResultSet> read_activities_by_club(int club_id);

    /**
     * @brief Reads a specific activity by its ID, verifying club membership.
     * @param club_id The ID of the club to which the activity must belong.
     * @param act_id The ID of the activity to be read.
     * @return std::unique_ptr<sql::ResultSet> Result set if valid, nullptr if not.
     */
    std::unique_ptr<sql::ResultSet> read_activity_by_id(int club_id, int act_id);

    /**
     * @brief Updates the details of an activity for a club, checking club membership.
     * @param club_id The ID of the club to which the activity must belong.
     * @param act_id The ID of the activity to be updated.
     * @param updates A map of column names and new values for the update.
     * @return True If the activity was successfully updated, false otherwise.
     */
    bool update_activity_for_club(int club_id, int act_id, const std::map<std::string, std::string> &updates);

    /**
     * @brief Deletes an activity associated with a club, checking club membership.
     * @param club_id The ID of the club to which the activity must belong.
     * @param act_id The ID of the activity to be deleted.
     * @return True If the activity was successfully deleted, false otherwise.
     */
    bool delete_activity_for_club(int club_id, int act_id);

    /**
     * @brief Reads activities by title for a specific club.
     *
     * @param club_id The ID of the club.
     * @param act_title The title of the activity to search for.
     * @return std::unique_ptr<sql::ResultSet> The result set containing matching activities.
     */
    std::unique_ptr<sql::ResultSet> read_activity_by_title(int club_id, const std::string& act_title);

    /**
     * @brief Reads activities within a specified period for a specific club.
     *
     * @param club_id The ID of the club.
     * @param from_date The start date of the period.
     * @param to_date The end date of the period.
     * @return std::unique_ptr<sql::ResultSet> The result set containing activities within the specified period.
     */
    std::unique_ptr<sql::ResultSet> read_activity_by_period(int club_id, const std::string& from_date, const std::string& to_date);
};