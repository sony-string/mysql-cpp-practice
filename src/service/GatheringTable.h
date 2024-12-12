#pragma once

#include <memory>
#include <string>
#include <map>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>


#include "../utils.h"
#include "BasicTable.h"
#include "GatheringStudentTable.h"

/**
 * @class GatheringTable
 * @brief Manages CRUD operations for gatherings within an activity.
 *
 * Provides functionality to create, read, update, and delete information
 * about gatherings related to various activities.
 */
class GatheringTable : public BasicTable {
protected:
    GatheringStudentTable gathering_student_table;
public:
    /**
     * @brief Constructs a new Gathering Table object.
     * @param conn A shared pointer to an active SQL database connection.
     */
    GatheringTable(std::shared_ptr<sql::Connection> conn);

    /**
     * @brief Creates a new gathering record.
     * @param act_id The ID of the activity to which this gathering belongs.
     * @param gathering_name The name of the gathering.
     * @return True If the gathering was successfully created, false otherwise.     
     */
    bool create_gathering(int act_id, const std::string &gathering_name);
    
    /**
     * @brief Retrieves gatherings by activity ID.
     * @param act_id The ID of the activity whose gatherings are to be retrieved.
     * @return std::unique_ptr<sql::ResultSet> Pointer to the result set with the matching gatherings.
     */
    std::unique_ptr<sql::ResultSet> read_gathering_by_act_id(int act_id);
    
    /**
     * @brief Retrieves gatherings by gathering name.
     * @param gathering_name The name of the gathering to search for.
     * @return std::unique_ptr<sql::ResultSet> Pointer to the result set with the matching gatherings.
     */
    std::unique_ptr<sql::ResultSet> read_gathering_by_name(const std::string &gathering_name);
    
    /**
     * @brief Updates the name of a specific gathering.     
     * @param gathering_id The ID of the gathering to update.
     * @param new_name The new name for the gathering.
     * @return True If the gathering was successfully updated, false otherwise.
     */
    bool update_gathering_name(int gathering_id, const std::string &new_name);
    
    /**
     * @brief Deletes a specific gathering.     
     * @param gathering_id The ID of the gathering to delete.
     * @return True If the gathering was successfully deleted, false otherwise.     
     */
    bool delete_gathering(int gathering_id);

    /**
     * @brief Add a student to a gathering.     
     * @param student_id The ID of the student.
     * @param gathering_id The ID of the gathering.
     * @return True If the operation was successful, false otherwise.     
     */
    bool add_student_to_gathering(int student_id, int gathering_id);

    /**
     * @brief Reads all students associated with any gathering.     
     * @param gathering_id The ID of the gathering.
     * @return Result set containing all gathering-student associations.
     */
    std::unique_ptr<sql::ResultSet> read_all_students_from_gathering(int gathering_id);

    /**
     * @brief Disassociates a student from a gathering by deleting a record.     
     * @param student_id The ID of the student.
     * @param gathering_id The ID of the gathering.
     * @return True If the operation was successful, false otherwise.
     */
    bool delete_student_from_gathering(int student_id, int gathering_id);
};