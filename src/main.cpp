#include <mysql/mysql.h>
#include <mysql_error.h>
#include <iostream>
#include <cstdlib>

void search_books(MYSQL *conn) {
    if (mysql_query(conn, "SELECT * FROM Book")) {
        std::cerr << "Query error: " << mysql_error(conn) << std::endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);

    if (res == nullptr) {
        std::cerr << "Store result error: " << mysql_error(conn) << std::endl;
        return;
    }

    int num_fields = mysql_num_fields(res);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        for(int i = 0; i < num_fields; i++) {
            std::cout << (row[i] ? row[i] : "NULL") << "\t";
        }
        std::cout << std::endl;
    }

    mysql_free_result(res);
}

void insert_book(MYSQL *conn, int bookid, const std::string &bookname, const std::string &publisher, int price) {
    std::string query = "INSERT INTO Book (bookid, bookname, publisher, price) VALUES (" +
        std::to_string(bookid) + ", '" + bookname + "', '" + publisher + "', " + std::to_string(price) + ")";
    
    if (mysql_query(conn, query.c_str())) {
        return;
    }

    std::cout << "Book inserted successfully" << std::endl;
}

void deleteBook(MYSQL *conn, int bookid) {
    std::string query = "DELETE FROM Book WHERE bookid = " + std::to_string(bookid);
    
    if (mysql_query(conn, query.c_str())) {
        return;
    }

    std::cout << "Book deleted successfully" << std::endl;
}


int main() {
    MYSQL *conn;

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

    while (true) {
        int query_num;
        std::cout << "1. search\t2. insert\t3. delete" << std::endl;
        std::cin >> query_num;

        if (query_num == 1) {
            search_books(conn);
        } else if (query_num == 2) {
            int bookid, price;
            std::string bookname, publisher;

            std::cout << "<bookid> <bookname> <publisher> <price>" << std::endl;
            std::cin >> bookid >> bookname >> publisher >> price;
            insert_book(conn, bookid, bookname, publisher, price);
        } else if (query_num == 3) {
             int bookid;

             std::cout << "<bookid>" << std::endl;
             std::cin >> bookid;
             deleteBook(conn, bookid);
        }
    }
    
    
    mysql_close(conn);

    return 0;
}