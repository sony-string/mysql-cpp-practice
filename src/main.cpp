#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include <iostream>
#include <memory>
#include <vector>

#include "service/ClubStudentTable.h"
#include "service/ClubTable.h"
#include "service/ProfessorTable.h"
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
            int search_option;
            std::cout << "Search by: 0. all  1. student_name" << std::endl;
            std::cin >> search_option;

            if (std::cin.fail() || search_option < 0 || search_option > 2) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            if (search_option == 0) {
                auto search_res = student_table.read_all_student();
                if (search_res)
                    print_result_set(search_res);
            } else if (search_option == 1) {
                clear_cin_buffer();
                std::string student_name;
                std::cout << "name = ";
                std::getline(std::cin, student_name);

                auto search_res = student_table.read_student_by_field("name", student_name);
                if (search_res)
                    print_result_set(search_res);
            }
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

void club_manage_menu(ClubTable &club_table, ClubStudentTable &club_student_table, int club_id) {
    while (true) {
        int query_num;
        std::cout << "0. info  \t1. see info\t2. manage\t3.submit Result\t4. update club details\n5. Return to Back" << std::endl;
        std::cin >> query_num;

        if (std::cin.fail()) {
            clear_cin_error();
        }

        if (query_num == 5)
            break;
        
        if (query_num == 4) {
            std::cout << "Update for: 1. Name  2. Budget  3. Professor ID" << std::endl;
            int update_option;
            std::cin >> update_option;

            if (std::cin.fail() || update_option < 1 || update_option > 3) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            if (update_option == 1) {
                clear_cin_buffer();
                std::string new_name;
                std::cout << "New Name = ";
                std::getline(std::cin, new_name);

                club_table.update_club_name(club_id, new_name);
            } else if (update_option == 2) {
                double new_budget;
                std::cout << "New Budget = ";
                std::cin >> new_budget;

                if (std::cin.fail()) {
                    clear_cin_error();
                    wrong_input_log.log();
                    continue;
                }

                club_table.update_club_budget(club_id, new_budget);
            } else if (update_option == 3) {
                int new_prof_id;
                std::cout << "New Professor ID = ";
                std::cin >> new_prof_id;

                if (std::cin.fail()) {
                    clear_cin_error();
                    wrong_input_log.log();
                    continue;
                }

                club_table.update_club_prof_id(club_id, new_prof_id);
            }
        }
    }
}

void club_menu(ClubTable &club_table, ClubStudentTable &club_student_table) {
    while (true) {
        int query_num;
        std::cout << "0. info  \t1. search\t2. create\t3. delete\t 4. manage a club\n5. Return to Menu" << std::endl;
        std::cin >> query_num;

        if (std::cin.fail()) {
            clear_cin_error();
        }

        if (query_num == 5)
            break;

        if (query_num == 0) {
            club_table.basic_show();
        } else if (query_num == 1) {
            int search_option;
            std::cout << "Search by: 0. all  1. club_id  2. club_name  3. prof_id  4. loc_id  5. loc_name" << std::endl;
            std::cin >> search_option;

            if (std::cin.fail() || search_option < 0 || search_option > 5) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            if (search_option == 0) {
                auto search_res = club_table.read_all_club();
                if (search_res)
                    print_result_set(search_res);
            } else if (search_option == 1) {
                int club_id;
                std::cout << "club_id = ";
                std::cin >> club_id;

                if (std::cin.fail()) {
                    clear_cin_error();
                    wrong_input_log.log();
                    continue;
                }

                auto search_res = club_table.read_club_by_id(club_id);
                if (search_res)
                    print_result_set(search_res);
            } else if (search_option == 2) {
                std::string club_name;
                std::cout << "club_name = ";
                std::cin >> club_name;

                auto search_res = club_table.read_club_by_name(club_name);
                if (search_res)
                    print_result_set(search_res);
            } else if (search_option == 3) {
                int prof_id;
                std::cout << "prof_id = ";
                std::cin >> prof_id;

                if (std::cin.fail()) {
                    clear_cin_error();
                    wrong_input_log.log();
                    continue;
                }

                auto search_res = club_table.read_club_by_prof_id(prof_id);
                if (search_res)
                    print_result_set(search_res);
            } else if (search_option == 4) {
                int loc_id;
                std::cout << "loc_id = ";
                std::cin >> loc_id;

                if (std::cin.fail()) {
                    clear_cin_error();
                    wrong_input_log.log();
                    continue;
                }

                auto search_res = club_table.read_club_by_location_id(loc_id);
                if (search_res)
                    print_result_set(search_res);
            } else if (search_option == 5) {
                std::string loc_name;
                std::cout << "loc_name = ";
                std::cin >> loc_name;

                auto search_res = club_table.read_club_by_location_name(loc_name);
                if (search_res)
                    print_result_set(search_res);
            }

        } else if (query_num == 2) {
            clear_cin_buffer();
            std::string club_name;
            double budget;
            int prof_id;

            std::cout << "club_name = ";
            std::getline(std::cin, club_name);

            std::cout << "budget = ";
            std::cin >> budget;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            std::cout << "prof_id = ";
            std::cin >> prof_id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            club_table.create_club(club_name, budget, prof_id);
        } else if (query_num == 3) {
            int club_id;
            std::cout << "club_id = ";
            std::cin >> club_id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            club_table.delete_club(club_id);
        } else if (query_num == 4) {
            int club_id;
            std::cout << "Select club to manage\nclub_id = ";
            std::cin >> club_id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            auto res = club_table.read_club_by_id(club_id);
            if (res) {
                club_manage_menu(club_table, club_student_table, club_id);
            } else {
                wrong_input_log.log();
            }
        }
    }
}

void professor_menu(ProfessorTable &professor_table) {
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
            professor_table.basic_show();
        } else if (query_num == 1) {
            int search_option;
            std::cout << "Search by: 0. all 1. prof_id  2. club_id" << std::endl;
            std::cin >> search_option;

            if (std::cin.fail() || search_option < 0 || search_option > 2) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            if (search_option == 0) {
                auto search_res = professor_table.read_all_professor();
                if (search_res)
                    print_result_set(search_res);
            } else if (search_option == 1) {
                int prof_id;
                std::cout << "prof_id = ";
                std::cin >> prof_id;

                if (std::cin.fail()) {
                    clear_cin_error();
                    wrong_input_log.log();
                    continue;
                }

                auto search_res = professor_table.read_professor_by_id(prof_id);
                if (search_res)
                    print_result_set(search_res);
            } else if (search_option == 2) {
                int club_id;
                std::cout << "club_id = ";
                std::cin >> club_id;

                if (std::cin.fail()) {
                    clear_cin_error();
                    wrong_input_log.log();
                    continue;
                }

                auto search_res = professor_table.read_professor_by_club_id(club_id);
                if (search_res)
                    print_result_set(search_res);
            }

        } else if (query_num == 2) {
            clear_cin_buffer();
            std::string name;
            std::cout << "name = ";
            std::getline(std::cin, name);

            professor_table.create_professor(name);
        } else if (query_num == 3) {
            int prof_id;
            std::cout << "prof_id = ";
            std::cin >> prof_id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            professor_table.delete_professor(prof_id);
        } else if (query_num == 4) {
            int prof_id;
            std::string new_name;

            std::cout << "Select professor to update\nprof_id = ";
            std::cin >> prof_id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }
            clear_cin_buffer();
            std::cout << "New Name = ";
            std::getline(std::cin, new_name);

            professor_table.update_professor_name(prof_id, new_name);
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
    ClubTable club_table(con);
    ClubStudentTable club_student_table(con);
    ProfessorTable professor_table(con);

    Logger(ll_info, "Initiation Done!").log();

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
            club_menu(club_table, club_student_table);
            break;
        case 2:
            student_menu(student_table);
            break;
        case 5:
            professor_menu(professor_table);
        default:
            break;
        }
    }

    return 0;
}