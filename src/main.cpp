#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include <iostream>
#include <vector>
#include <memory>

#include "utils.h"
#include "service/StudentTable.h"


int main() {
    sql::Driver *driver = get_driver_instance();

    const std::string server = std::getenv("MYSQL_SERVER");
    const std::string user = std::getenv("MYSQL_USER");
    const std::string password = std::getenv("MYSQL_PASSWORD");
    const std::string database = std::getenv("MYSQL_DATABASE");

    std::shared_ptr<sql::Connection> con;

    try {
        con = std::shared_ptr<sql::Connection>(driver->connect("tcp://" + server, user, password));
        con->setSchema(database);
        Logger(ll_info, "Successfully Connected to MySQL").log();
    } catch (sql::SQLException &e) {
        Logger(ll_critical, std::string(e.what())).log();
        exit(EXIT_FAILURE);
    }

    StudentTable student_table(con);
    
    while (true) {
        int query_num;
        std::cout << "0. info  \t1. search\t2. insert\t3. delete\t 4. update" << std::endl;
        std::cin >> query_num;

        if (std::cin.fail()) {
            clear_cin_error();
        }

        if (query_num == 0) {
            student_table.basic_show();
        } else if (query_num == 1) {
            std::string student_name;
            std::cout << "name = ";
            std::cin >> student_name;

            auto search_res = student_table.read_student_by_field("name", student_name);                 
            if (search_res)
                print_result_set(search_res);
        } else if (query_num == 2) {
            std::string student_name, dep;
            std::cout << "name = ";
            std::cin >> student_name;

            std::cout << "department = ";
            std::cin >> dep;            

            student_table.create_student(student_name, dep);
        }
    }

    return 0;
}