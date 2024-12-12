#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include <iostream>
#include <memory>
#include <vector>

#include "service/ClubStudentTable.h"
#include "service/ClubTable.h"
#include "service/GatheringTable.h"
#include "service/ProfessorTable.h"
#include "service/StudentTable.h"
#include "utils.h"

static Logger wrong_input_log = Logger(ll_error, "Incorrect Input");

void student_menu(StudentTable &student_table) {
    while (true) {
        int query_num;
        std::cout << "0. info  1. search  2. create  3. delete 4. update name  5. Return to Menu" << std::endl;
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

void club_members_menu(ClubTable &club_table, GatheringTable &gathering_table, int club_id) {
    while (true) {
        int query_num;
        std::cout << "\n\n<< Members >>\n1. Search  2. Add  3. Delete  4. Return to back" << std::endl;
        std::cin >> query_num;

        if (std::cin.fail()) {
            clear_cin_error();
        }

        if (query_num == 4)
            break;

        if (query_num == 1) {
            int search_option;
            std::cout << "Search by: 0. all  1. name" << std::endl;
            std::cin >> search_option;

            if (std::cin.fail() || search_option < 0 || search_option > 1) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            if (search_option == 0) {
                auto search_res = club_table.read_members_by_club_id(club_id);
                if (search_res) {
                    print_result_set(search_res);
                }
            } else if (search_option == 1) {
                clear_cin_buffer();
                std::string search_name;
                std::cout << "Search for Name = ";
                std::getline(std::cin, search_name);

                auto search_res = club_table.read_members_by_name_in_club(club_id, search_name);
                if (search_res) {
                    print_result_set(search_res);
                }
            }
        } else if (query_num == 2) {
            int student_id;
            std::cout << "student_id = ";
            std::cin >> student_id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            club_table.add_member(club_id, student_id);
        } else if (query_num == 3) {
            int student_id;
            std::cout << "student_id = ";
            std::cin >> student_id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            club_table.delete_member(club_id, student_id);
        }
    }
}

void gathering_menu(GatheringTable &gathering_table, int gathering_id) {
    while (true) {
        int option;
        std::cout << "\n\n << Gathering >>\nChoose an option: 1. Add Member  2. Remove Member  3. View Members  4. Return to Back" << std::endl;
        std::cin >> option;

        if (std::cin.fail() || option < 1 || option > 4) {
            clear_cin_error();
            wrong_input_log.log();
            continue;
        }

        if (option == 4) break;

        if (option == 1) {
            int student_id;
            std::cout << "Enter student_id to add: ";
            std::cin >> student_id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            gathering_table.add_student_to_gathering(student_id, gathering_id);
        } else if (option == 2) {
            int student_id;
            std::cout << "Enter student_id to remove: ";
            std::cin >> student_id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            gathering_table.delete_student_from_gathering(student_id, gathering_id);
        } else if (option == 3) {
            auto students = gathering_table.read_all_students_from_gathering(gathering_id);
            if (students) {
                print_result_set(students);
            } else {
                std::cout << "No students found for the given gathering." << std::endl;
            }
        }
    }
}

void club_activities_menu(ClubTable &club_table, GatheringTable &gathering_table, int club_id) {
    while (true) {
        int query_num;
        std::cout << "\n\n<< Activities >>\n1. Search  2. Add  3. Update  4. Delete  5. Gathering  6. Return to back" << std::endl;
        std::cin >> query_num;

        if (std::cin.fail()) {
            clear_cin_error();
            continue;
        }

        if (query_num == 6)
            break;

        if (query_num == 1) {
            int search_option;
            std::cout << "Search by: 0. all  1. activity_id  2. title  3. period" << std::endl;
            std::cin >> search_option;

            if (std::cin.fail() || search_option < 0 || search_option > 3) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            if (search_option == 0) {
                auto search_res = club_table.read_activities_by_club(club_id);
                if (search_res) {
                    print_result_set(search_res);
                }
            } else if (search_option == 1) {
                int act_id;
                std::cout << "activity_id = ";
                std::cin >> act_id;

                if (std::cin.fail()) {
                    clear_cin_error();
                    wrong_input_log.log();
                    continue;
                }

                auto search_res = club_table.read_activity_by_id(club_id, act_id);
                if (search_res) {
                    print_result_set(search_res);
                }
            } else if (search_option == 2) {
                clear_cin_buffer();
                std::string act_title;
                std::cout << "activity_title = ";
                std::getline(std::cin, act_title);

                auto search_res = club_table.read_activity_by_title(club_id, act_title);
                if (search_res) {
                    print_result_set(search_res);
                }
            } else if (search_option == 3) {
                clear_cin_buffer();
                std::string from_date, to_date;
                std::cout << "From date (YYYY-MM-DD) = ";
                std::getline(std::cin, from_date);

                std::cout << "To date (YYYY-MM-DD) = ";
                std::getline(std::cin, to_date);

                auto search_res = club_table.read_activity_by_period(club_id, from_date, to_date);
                if (search_res) {
                    print_result_set(search_res);
                }
            }
        } else if (query_num == 2) {
            clear_cin_buffer();
            std::string act_title, start_date, end_date;

            std::cout << "activity_title = ";
            std::getline(std::cin, act_title);

            std::cout << "start_date (YYYY-MM-DD, default: NOW) = ";
            std::getline(std::cin, start_date);

            std::cout << "end_date (YYYY-MM-DD, default: 2099-12-31) = ";
            std::getline(std::cin, end_date);

            club_table.create_activity_for_club(club_id, act_title, start_date, end_date);

        } else if (query_num == 3) {
            int act_id;
            std::cout << "activity_id = ";
            std::cin >> act_id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            std::map<std::string, std::string> updates;
            clear_cin_buffer();
            std::string field_to_update, new_value;

            while (true) {
                std::cout << "Enter field to update (title, start_date, end_date or 'done' to finish): ";
                std::cin >> field_to_update;

                if (field_to_update == "done")
                    break;

                std::cout << "New value for " << field_to_update << ": ";
                std::cin >> new_value;

                updates[field_to_update] = new_value;
            }

            club_table.update_activity_for_club(club_id, act_id, updates);

        } else if (query_num == 4) {
            int act_id;
            std::cout << "activity_id = ";
            std::cin >> act_id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            club_table.delete_activity_for_club(club_id, act_id);
        } else if (query_num == 5) {
            int activity_id;
            std::cout << "Enter activity_id: ";
            std::cin >> activity_id;

            if (std::cin.fail()) {
                clear_cin_error();
                wrong_input_log.log();
                continue;
            }

            if (!club_table.validate_activity_belongs_to_club(club_id, activity_id)) {
                std::cout << "The activity does not belong to this club. Please enter a valid activity." << std::endl;
                continue;
            }

            auto gatherings = gathering_table.read_gathering_by_act_id(activity_id);
            if (!gatherings || gatherings->rowsCount() == 0) {
                std::cout << "No gatherings found for this activity." << std::endl;
                char create_option;
                std::cout << "Would you like to create a new gathering? (y/n): ";
                std::cin >> create_option;
                clear_cin_buffer();

                if (create_option == 'y' || create_option == 'Y') {
                    std::string gathering_name;
                    std::cout << "Enter gathering name: ";
                    std::getline(std::cin, gathering_name);

                    gathering_table.create_gathering(activity_id, gathering_name);
                }
            } else {
                gatherings->next();
                int gathering_id = gatherings->getInt("gathering_id");
                gathering_menu(gathering_table, gathering_id);
            }
        }
    }
}

void club_manage_menu(ClubTable &club_table, GatheringTable &gathering_table, int club_id) {
    while (true) {
        int query_num;
        std::cout << "1. Manage\t2. Submit Result\t4. Return to Back" << std::endl;
        std::cin >> query_num;

        if (std::cin.fail()) {
            clear_cin_error();
        }

        if (query_num == 4)
            break;

        if (query_num == 1) {
            while (true) {
                std::cout << "1. Members  2. Activities  3. Details  4. Return to Back" << std::endl;
                int info_option;
                std::cin >> info_option;

                if (std::cin.fail()) {
                    clear_cin_error();
                }

                if (info_option == 4)
                    break;

                if (info_option == 1) {
                    club_members_menu(club_table, gathering_table, club_id);
                } else if (info_option == 2) {
                    club_activities_menu(club_table, gathering_table, club_id);
                } else if (info_option == 3) {
                    auto search_res = club_table.read_club_by_id(club_id);
                    if (search_res)
                        print_result_set(search_res);

                    std::cout << "Update for: 1. Name  2. Budget  3. Professor ID  4. Return to back" << std::endl;
                    int update_option;
                    std::cin >> update_option;

                    if (std::cin.fail() || update_option < 1 || update_option > 4) {
                        clear_cin_error();
                        wrong_input_log.log();
                        continue;
                    }

                    if (update_option == 4) {
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
    }
}

void club_menu(ClubTable &club_table, GatheringTable &gathering_table) {
    while (true) {
        int query_num;
        std::cout << "0. info  1. search  2. create  3. delete  4. manage a club  5. Return to Menu" << std::endl;
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
        std::cout << "0. info  1. search  2. create  3. delete  4. update name  5. Return to Menu" << std::endl;
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
    GatheringTable gathering_table(con);

    Logger(ll_info, "Initiation Done!").log();

    while (true) {
        int query_num;
        std::cout << "\n<<Select Table for Service>>\n\n";
        std::cout << "1. Club\t\t2. Student\n"
                  << "3. Professor\t4. Result\n5. Location\t6. Equipment\n";

        std::cin >> query_num;

        if (std::cin.fail()) {
            clear_cin_error();
            wrong_input_log.log();
            continue;
        }

        switch (query_num) {
        case 1:
            club_menu(club_table, gathering_table);
            break;
        case 2:
            student_menu(student_table);
            break;
        case 3:
            professor_menu(professor_table);
        default:
            break;
        }
    }

    return 0;
}