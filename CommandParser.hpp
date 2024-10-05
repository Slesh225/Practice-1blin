#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP

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
                MyVector<std::string> columns;
                std::string column;
                while (iss >> column) {
                    columns.push_back(column);
                }
                db.createTable(tableName, columns);
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
                MyVector<std::string> values;
                std::string value;
                while (iss >> value) {
                    values.push_back(value);
                }
                db.insertInto(tableName, values);
            }
            else {
                std::cerr << "Error: Missing 'INTO' keyword." << std::endl;
            }
        }
        else if (keyword == "DELETE" || keyword == "delete") {
            std::string from;
            iss >> from;
            if (from == "FROM" || from == "from") {
                std::string tableName;
                iss >> tableName;
                if (tableName.empty()) {
                    std::cerr << "Error: Missing table name." << std::endl;
                    return;
                }
                MyVector<std::string> values;
                std::string value;
                while (iss >> value) {
                    values.push_back(value);
                }
                db.deleteFrom(tableName, values);
            }
            else {
                std::cerr << "Error: Missing 'FROM' keyword." << std::endl;
            }
        }
        else if (keyword == "SELECT" || keyword == "select") {
            std::string from;
            iss >> from;
            if (from == "FROM" || from == "from") {
                std::string tableName;
                iss >> tableName;
                if (tableName.empty()) {
                    std::cerr << "Error: Missing table name." << std::endl;
                    return;
                }
                db.selectFrom(tableName);
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

#endif // COMMANDPARSER_HPP