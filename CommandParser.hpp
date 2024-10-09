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
            std::string column1, column2, from;
            iss >> column1;
            iss >> from;
            if (from != "FROM" && from != "from") {
                std::cerr << "Error: Missing 'FROM' keyword." << std::endl;
                return;
            }
            std::string tableName;
            iss >> tableName;
            db.selectFrom(tableName, column1, column2);
        }
        else if (keyword == "DELETE" || keyword == "delete") {
            std::string from;
            iss >> from;
            if (from != "FROM" && from != "from") {
                std::cerr << "Error: Missing 'FROM' keyword." << std::endl;
                return;
            }
            std::string tableName;
            iss >> tableName;
            std::string where;
            iss >> where;
            if (where != "WHERE" && where != "where") {
                std::cerr << "Error: Missing 'WHERE' keyword." << std::endl;
                return;
            }
            std::string conditionColumn, conditionValue;
            iss >> conditionColumn;
            iss >> conditionValue;

            // Extract table name and column for the condition
            std::string columnName = conditionColumn.substr(conditionColumn.find('.') + 1);
            std::string tableCondition = conditionColumn.substr(0, conditionColumn.find('.'));

            if (tableName == tableCondition) {
                db.deleteFrom(tableName, conditionValue);
            }
            else {
                std::cerr << "Error: Table names do not match." << std::endl;
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
