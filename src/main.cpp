#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include <iostream>
#include <memory>
#include <vector>

#include "service/StudentTable.h"
#include "utils.h"

static Logger wrong_input_log = Logger(ll_error, "Incorrect Input");

void student_menu(StudentTable &student_table) {
    while (true) {
        int query_num;
        std::cout << "0. info  \t1. search\t2. create\t3. delete\t 4. update name\n5. Return to Menu" << std::endl;
        std::cin >> query_num;

        if (std::cin.fail()) {
            clear_cin_error();
        }

        if (query_num == 5)
            break;

        if (query_num == 0) {
            student_table.basic_show();
        } else if (query_num == 1) {
            char ans;
            std::cout << "show all students? y/n : ";
            std::cin >> ans;
            clear_cin_buffer();
            if (ans != 'y' && ans != 'n' && ans != 'Y' && ans != 'N')        
                continue;

            if (ans == 'y' || ans == 'Y') {
                auto search_res = student_table.read_all_student();
                if (search_res)
                    print_result_set(search_res);
                continue;
            }

            std::string student_name;
            std::cout << "name = ";
            std::cin >> student_name;

            auto search_res = student_table.read_student_by_field("name", student_name);
            if (search_res)
                print_result_set(search_res);
        } else if (query_num == 2) {
            clear_cin_buffer();
            std::string student_name, dep;
            std::cout << "name = ";
            std::getline(std::cin, student_name);

            std::cout << "department = ";
            std::getline(std::cin, dep);

            student_table.create_student(student_name, dep);
        } else if (query_num == 3) {
            int id;
            std::cout << "student_id = ";
            std::cin >> id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            student_table.delete_student_by_id(id);
        } else if (query_num == 4) {            
            int id;
            std::string new_name;

            std::cout << "Select student to update\nstudent_id = ";
            std::cin >> id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }
            clear_cin_buffer();
            std::cout << "New Name = ";
            std::getline(std::cin, new_name);

            student_table.update_student_name(id, new_name);
        }
    }
}

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

    Logger(ll_info, "Initiation").log();
    StudentTable student_table(con);

    while (true) {
        int query_num;
        std::cout << "\n<<Select Table for Service>>\n\n";
        std::cout << "1. Club\t\t2. Student\n3. Activity\t4. Gathering\n"
                  << "5. Professor\t6. Result\n7. Location\t8. Equipment\n";

        std::cin >> query_num;

        if (std::cin.fail()) {
            clear_cin_error();
            wrong_input_log.log();
            continue;
        }

        switch (query_num) {
        case 1:
            break;
        case 2:
            student_menu(student_table);
            break;
        default:
            break;
        }
    }

    return 0;
}