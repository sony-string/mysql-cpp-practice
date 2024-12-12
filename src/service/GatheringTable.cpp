#include "GatheringTable.h"

GatheringTable::GatheringTable(std::shared_ptr<sql::Connection> conn) : BasicTable("Gathering", conn), gathering_student_table(conn) {}

bool GatheringTable::create_gathering(int act_id, const std::string &gathering_name) {
    try {
        std::string query = "INSERT INTO Gathering (act_id, gathering_name) VALUES (?, ?)";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, act_id);
        pstmt->setString(2, gathering_name);
        pstmt->executeUpdate();
        Logger(ll_info, "executeQuery: " + query).log();
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "Error in create_gathering: " + std::string(e.what())).log();
        return false;
    }
    return true;
}

std::unique_ptr<sql::ResultSet> GatheringTable::read_gathering_by_act_id(int act_id) {
    try {
        std::string query = "SELECT * FROM Gathering WHERE act_id = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, act_id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();
        return res;
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "Error in read_gathering_by_act_id: " + std::string(e.what())).log();
        return nullptr;
    }
}

std::unique_ptr<sql::ResultSet> GatheringTable::read_gathering_by_name(const std::string &gathering_name) {
    try {
        std::string query = "SELECT * FROM Gathering WHERE gathering_name LIKE ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setString(1, '%' + gathering_name + '%');
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();
        return res;
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "Error in read_gathering_by_name: " + std::string(e.what())).log();
        return nullptr;
    }
}

bool GatheringTable::update_gathering_name(int gathering_id, const std::string &new_name) {
    try {
        std::string query = "UPDATE Gathering SET gathering_name = ? WHERE gathering_id = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setString(1, new_name);
        pstmt->setInt(2, gathering_id);
        pstmt->executeUpdate();
        Logger(ll_info, "executeQuery: " + query).log();
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "Error in update_gathering_name: " + std::string(e.what())).log();
        return false;
    }
    return true;
}

bool GatheringTable::delete_gathering(int gathering_id) {
    try {
        std::string query = "DELETE FROM Gathering WHERE gathering_id = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, gathering_id);
        pstmt->executeUpdate();
        Logger(ll_info, "executeQuery: " + query).log();
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "Error in delete_gathering: " + std::string(e.what())).log();
        return false;
    }
    return true;
}

bool GatheringTable::add_student_to_gathering(int student_id, int gathering_id) {
    return gathering_student_table.create_gathering_student(student_id, gathering_id);
}

std::unique_ptr<sql::ResultSet> GatheringTable::read_all_students_from_gathering(int gathering_id) {
    try {
        std::string query = "SELECT * FROM Student AS s WHERE s.student_id IN (SELECT gs.student_id FROM Gathering_Student AS gs WHERE gs.gathering_id = ?)";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, gathering_id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();
        return res;
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "Error in delete_gathering: " + std::string(e.what())).log();
        return nullptr;
    }
}

bool GatheringTable::delete_student_from_gathering(int student_id, int gathering_id) {
    return gathering_student_table.delete_gathering_student(student_id, gathering_id);
}