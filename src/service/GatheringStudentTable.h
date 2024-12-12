#pragma once

#include <memory>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>


#include "../utils.h"
#include "BasicTable.h"

/**
 * @class GatheringStudentTable
 * @brief Manages CRUD operations for the relationship between students and gatherings.
 *
 * Provides functionality to create, read, and delete records indicating students' participation in gatherings.
 */
class GatheringStudentTable : public BasicTable {
public:
    /**
     * @brief Constructs a new Gathering Student Table object.
     * @param conn A shared pointer to an active SQL database connection.
     */
    GatheringStudentTable(std::shared_ptr<sql::Connection> conn);

    /**
     * @brief Creates a new record associating a student with a gathering.
     * @param student_id The ID of the student.
     * @param gathering_id The ID of the gathering.
     * @return True If the record was successfully created, false otherwise.     
     */
    bool create_gathering_student(int student_id, int gathering_id);

    /**
     * @brief Reads all records from the Gathering_Student table.
     * @return std::unique_ptr<sql::ResultSet> Result set containing all records.
     */
    std::unique_ptr<sql::ResultSet> read_all_gathering_students();

    /**
     * @brief Deletes a record associating a student with a gathering.
     * @param student_id The ID of the student.
     * @param gathering_id The ID of the gathering.
     * @return True If the record was successfully deleted, false otherwise.     
     */
    bool delete_gathering_student(int student_id, int gathering_id);
};