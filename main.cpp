#include <iostream>
#include <sstream>
#include <string>
#include "Database.hpp"

class CommandParser {
public:
    CommandParser(Database& db) : db(db) {}

    void parse(const std::string& command) {
        std::istringstream iss(command);
        std::string keyword;
        iss >> keyword;

        if (keyword == "CREATE" || keyword == "create") {
            std::string table;
            iss >> table;
            if (table == "TABLE" || table == "table") {
                std::string tableName;
                iss >> tableName;
                if (tableName.empty()) {
                    std::cerr << "Error: Missing table name." << std::endl;
                    return;
                }
                db.createTable(tableName);
            }
            else {
                std::cerr << "Error: Missing 'TABLE' keyword." << std::endl;
            }
        }
        else if (keyword == "INSERT" || keyword == "insert") {
            std::string into;
            iss >> into;
            if (into == "INTO" || into == "into") {
                std::string tableName;
                iss >> tableName;
                if (tableName.empty()) {
                    std::cerr << "Error: Missing table name." << std::endl;
                    return;
                }
                std::string values;
                iss >> values;
                if (values != "VALUES" && values != "values") {
                    std::cerr << "Error: Missing 'VALUES' keyword." << std::endl;
                    return;
                }
                std::string key, value;
                iss >> key >> value;
                db.insertInto(tableName, key, value);
            }
            else {
                std::cerr << "Error: Missing 'INTO' keyword." << std::endl;
            }
        }
        else if (keyword == "SELECT" || keyword == "select") {
            std::string column1, column2;
            iss >> column1 >> column2;
            std::string from;
            iss >> from;
            if (from == "FROM" || from == "from") {
                std::string tableName1, tableName2;
                iss >> tableName1 >> tableName2;
                if (tableName1.empty() || tableName2.empty()) {
                    std::cerr << "Error: Missing table names." << std::endl;
                    return;
                }
                db.selectFrom(tableName1, tableName2, column1, column2);
            }
            else {
                std::cerr << "Error: Missing 'FROM' keyword." << std::endl;
            }
        }
        else {
            std::cerr << "Error: Unknown command." << std::endl;
        }
    }

private:
    Database& db;
};

int main() {
    Database db;
    CommandParser parser(db);
    std::string command;

    std::cout << "Enter SQL-like commands (type 'exit' to quit):" << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        }

        parser.parse(command);
    }

    return 0;
}
