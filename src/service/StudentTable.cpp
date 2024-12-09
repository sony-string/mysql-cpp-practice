#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "BasicTable.h"
#include "../utils.h"
#include "StudentTable.h"

StudentTable::StudentTable(std::shared_ptr<sql::Connection> conn)
        : BasicTable("Student", conn) {}

bool StudentTable::create_student(const std::string &name, const std::string &department) {
    std::map<std::string, std::string> attributes;
    attributes["name"] = name;
    attributes["department"] = department;
    if (!basic_insert(attributes)) {
        Logger(ll_info, "Failed to insert student record: name=" + name + ", department=" + department).log();
        return false;
    }
    return true;
}

std::unique_ptr<sql::ResultSet> StudentTable::read_student_by_field(const std::string &field, const std::string &value) {
    std::map<std::string, std::string> conditions;
    conditions[field] = value;
    return basic_select(conditions);
}

bool StudentTable::update_student_name(int student_id, const std::string &new_name) {
    std::map<std::string, std::string> conditions;
    conditions["student_id"] = std::to_string(student_id);

    std::map<std::string, std::string> new_values;
    new_values["name"] = new_name;

    if (!basic_update(conditions, new_values)) {
        Logger(ll_info, "Failed to update student record: id=" + std::to_string(student_id)).log();
        return false;
    }
    return true;
}

bool StudentTable::delete_student_by_id(int student_id) {
    std::map<std::string, std::string> conditions;
    conditions["student_id"] = std::to_string(student_id);
    if (!basic_delete(conditions)) {
        Logger(ll_info, "Failed to delete student record: id=" + std::to_string(student_id)).log();
        return false;
    }
    return true;
}