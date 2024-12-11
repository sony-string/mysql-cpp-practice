#pragma once

#include <mysql_driver.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

/**
 * @brief Represents a basic database table for CRUD operations.
 */
class BasicTable {
protected:
    /**
     * @brief Name of the table this object manages.     
     */
    std::string table_name;

    /**
     * @brief Shared database connection.     
     */
    std::shared_ptr<sql::Connection> con;

    /**
     * @brief List of columns of this table.     
     */
    std::vector<std::string> columns;
    
    /**
     * @brief Constructs a new BasicTable object.
     * @param name The name of the table to manage.
     * @param conn A shared pointer to an active database connection.
     */
    BasicTable(std::string name, std::shared_ptr<sql::Connection> conn);

    /**
     * @brief Inserts a tuple into the table using given attributes.
     * @param attributes A map of column names to values.
     * @return True if insert was successful, false otherwise.
     */
    bool basic_insert(std::map<std::string, std::string> attributes);
    
    /**
     * @brief Deletes a tuple from the table matching the given conditions.
     * @param conditions A map of column names to values that identify the tuple.
     * @return True if a single matching tuple was deleted, false otherwise.
     */
    bool basic_delete(std::map<std::string, std::string> conditions);
    
    /**
     * @brief Updates a tuple in the table matching the given conditions.
     * @param conditions A map of column names to values that identify the tuple.
     * @param new_values A map of column names and their new values to update.
     * @return True if a single matching tuple was updated, false otherwise.
     */
    bool basic_update(std::map<std::string, std::string> conditions, 
                      std::map<std::string, std::string> new_values);
    
    /**
     * @brief Selects tuples from the table matching the given conditions.
     * @param conditions A map of column names to values that identify the tuple.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     * @retval nullptr An error occurred.
     */
    std::unique_ptr<sql::ResultSet> basic_select(std::map<std::string, std::string> conditions);
    
    /**
     * @brief Selects tuples from the table matching the given conditions.
     * @param conditions A map of column names to values that identify the tuple.
     * All fields need to be string, then this query will find values including value of conditions.
     * @return A unique pointer to a ResultSet containing the query results, or nullptr if an error occurred.
     * @retval nullptr An error occurred.
     */
    std::unique_ptr<sql::ResultSet> basic_string_select(std::map<std::string, std::string> conditions);   

    /// @brief Destructor for BasicTable.
    virtual ~BasicTable() {}

public:
    /**
     * @brief Displays the structure of the table.
     * @return True if the operation was successful, false otherwise.
     */
    bool basic_show();
};