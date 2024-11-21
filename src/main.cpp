#include <mysql/mysql.h>
#include <iostream>
#include <cstdlib>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    const char *server      = std::getenv("MYSQL_SERVER");  
    const char *user        = std::getenv("MYSQL_USER");  
    const char *password    = std::getenv("MYSQL_PASSWORD");
    const char *database    = std::getenv("MYSQL_DATABASE");   
    const int   portnum     = std::atoi(std::getenv("MYSQL_PORTNUM"));

    conn = mysql_init(nullptr);

    if (!mysql_real_connect(conn, server, user, password, database, portnum, nullptr, 0)) {
        std::cerr << "Connection error: " << mysql_error(conn) << std::endl;
        return 1;
    }

    if (mysql_query(conn, "SELECT * FROM Book")) {
        std::cerr << "Query error: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return 1;
    }

    res = mysql_store_result(conn);

    if (res == nullptr) {
        std::cerr << "Store result error: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return 1;
    }

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for(int i = 0; i < num_fields; i++) {
            std::cout << (row[i] ? row[i] : "NULL") << "\t";
        }
        std::cout << std::endl;
    }

    // 결과 해제
    mysql_free_result(res);
    mysql_close(conn);

    return 0;
}