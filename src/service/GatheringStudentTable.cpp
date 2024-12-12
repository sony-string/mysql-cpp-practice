#include "GatheringStudentTable.h"

GatheringStudentTable::GatheringStudentTable(std::shared_ptr<sql::Connection> conn)
    : BasicTable("Gathering_Student", conn) {}

bool GatheringStudentTable::create_gathering_student(int student_id, int gathering_id) {
    try {
        std::string query = "INSERT INTO Gathering_Student (student_id, gathering_id) VALUES (?, ?)";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, student_id);
        pstmt->setInt(2, gathering_id);
        pstmt->executeUpdate();
        Logger(ll_info, "executeQuery: " + query).log();
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "Error in create_gathering_student: " + std::string(e.what())).log();
        return false;
    }
    return true;
}

std::unique_ptr<sql::ResultSet> GatheringStudentTable::read_all_gathering_students() {
    try {
        std::string query = "SELECT * FROM Gathering_Student";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        Logger(ll_info, "executeQuery: " + query).log();
        return res;
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "Error in read_all_gathering_students: " + std::string(e.what())).log();
        return nullptr;
    }
}

bool GatheringStudentTable::delete_gathering_student(int student_id, int gathering_id) {
    try {
        std::string query = "DELETE FROM Gathering_Student WHERE student_id = ? AND gathering_id = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));
        pstmt->setInt(1, student_id);
        pstmt->setInt(2, gathering_id);
        pstmt->executeUpdate();
        Logger(ll_info, "executeQuery: " + query).log();
    } catch (const sql::SQLException &e) {
        Logger(ll_error, "Error in delete_gathering_student: " + std::string(e.what())).log();
        return false;
    }
    return true;
}