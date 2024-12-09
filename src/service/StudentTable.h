#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "BasicTable.h"
#include "../utils.h"

/**
 * @brief Represents the student table with specific CRUD operations.
 */
class StudentTable : public BasicTable {
public:
    /**
     * @brief Constructs a new StudentTable object.
     * @param conn A shared pointer to an active database connection.
     */
    StudentTable(std::shared_ptr<sql::Connection> conn);

    /**
     * @brief Creates a new student record.
     * @param name The name of the student.
     * @param department The department of the student.
     * @return True if the student was created successfully, false otherwise.
     */
    bool create_student(const std::string &name, const std::string &department);

    /**
     * @brief Reads a student record based on a specific field and value.
     * @param field The field to search by (e.g., student_id, name, department).
     * @param value The value to search for.
     * @return A unique pointer to a ResultSet containing the matching students' column names to their values, or nullptr if an error occurred.
     * @retval nullptr An error occurred.
     */
    std::unique_ptr<sql::ResultSet>  read_student_by_field(const std::string &field, const std::string &value);

    /**
     * @brief Updates a student's name based on their ID.
     * @param student_id The ID of the student to update.
     * @param new_name The new name for the student.
     * @return True if the student's name was updated successfully, false otherwise.
     */
    bool update_student_name(int student_id, const std::string &new_name);

    /**
     * @brief Deletes a student record based on their ID.
     * @param student_id The ID of the student to delete.
     * @return True if the student was deleted successfully, false otherwise.
     */
    bool delete_student_by_id(int student_id);
};