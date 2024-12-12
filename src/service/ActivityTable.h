#pragma once

#include <memory>
#include <map>
#include <string>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>

#include "../utils.h"
#include "BasicTable.h"

/**
 * @class ActivityTable
 * @brief Manages CRUD operations for activities within a club.
 *
 * Provides functionality to create, read, update, and delete information
 * about activities related to various clubs.
 */
class ActivityTable : public BasicTable {
protected:
    
public:
    /**
     * @brief Constructs a new Activity Table object.
     * @param conn A shared pointer to an active SQL database connection.
     */
    ActivityTable(std::shared_ptr<sql::Connection> conn);

    /**
     * @brief Creates a new activity record.
     * 
     * @param club_id The ID of the club.
     * @param act_title The title of the activity.
     * @param start_date The start date of the activity (defaults to current date).
     * @param end_date The end date of the activity (defaults to "2099-12-31").
     * @return true If the record was successfully created.
     * @return false If the record creation failed.
     */
    bool create_activity(int club_id, const std::string& act_title, 
                         const std::string& start_date = "", const std::string& end_date = "2099-12-31");

    /**
     * @brief Reads activities by club ID.
     * 
     * @param club_id The ID of the club whose activities are to be retrieved.
     * @return std::unique_ptr<sql::ResultSet> Result set containing the matching records.
     */
    std::unique_ptr<sql::ResultSet> read_activity_by_club_id(int club_id);

    /**
     * @brief Reads activities by activity title.
     * 
     * @param act_title The title of the activity to search for.
     * @param club_id The ID of the club whose activities are to be retrieved. If it is default(-1), do not use it.
     * @return std::unique_ptr<sql::ResultSet> Result set containing the matching records.
     */
    std::unique_ptr<sql::ResultSet> read_activity_by_title(const std::string& act_title, int club_id = -1);

    /**
     * @brief Reads activities within a specified period.
     * 
     * @param from_date Start date of the period.
     * @param to_date End date of the period.
     * @param club_id The ID of the club whose activities are to be retrieved. If it is default(-1), do not use it.
     * @return std::unique_ptr<sql::ResultSet> Result set containing the activities within the period.
     */
    std::unique_ptr<sql::ResultSet> read_activity_by_period(const std::string& from_date, const std::string& to_date, int club_id = -1);

    /**
     * @brief Reads a specific activity by its activity ID.
     * 
     * @param act_id The ID of the activity.
     * @return std::unique_ptr<sql::ResultSet> Result set containing the matching record.
     */
    std::unique_ptr<sql::ResultSet> read_activity_by_id(int act_id);

    /**
     * @brief Updates an activity's information.
     * 
     * @param act_id The ID of the activity to update.
     * @param updates A map of column names and their new values.
     * @return true If the update was successful.
     * @return false If the update failed.
     */
    bool update_activity(int act_id, const std::map<std::string, std::string>& updates);

    /**
     * @brief Deletes an activity.
     * 
     * @param act_id The ID of the activity to delete.
     * @return true If the activity was successfully deleted.
     * @return false If the deletion failed.
     */
    bool delete_activity(int act_id);
};