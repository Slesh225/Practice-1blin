#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Table {
public:
    std::unordered_map<std::string, std::string> data;

    void insert(const std::string& key, const std::string& value) {
        if (data.size() >= 1000) {
            std::cerr << "Error: Table is full." << std::endl;
            return;
        }
        data[key] = value;
    }

    void print() const {
        for (const auto& pair : data) {
            std::cout << pair.first << "\t" << pair.second << std::endl;
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
            return;
        }
        for (const auto& pair : data) {
            outFile << pair.first << "," << pair.second << std::endl;
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            std::cerr << "Error: Could not open file " << filename << " for reading." << std::endl;
            return;
        }
        std::string line;
        while (std::getline(inFile, line)) {
            std::istringstream ss(line);
            std::string key, value;
            std::getline(ss, key, ',');
            std::getline(ss, value, ',');
            data[key] = value;
        }
    }

    void deleteRow(const std::string& key) {
        if (data.erase(key) == 0) {
            std::cerr << "Error: Key not found." << std::endl;
        }
    }
};

class Database {
public:
    std::unordered_map<std::string, Table> tables;

    void createTable(const std::string& name) {
        if (tables.find(name) != tables.end()) {
            std::cerr << "Error: Table already exists." << std::endl;
            return;
        }
        tables[name] = Table();
    }

    void insertInto(const std::string& tableName, const std::string& key, const std::string& value) {
        if (tables.find(tableName) == tables.end()) {
            std::cerr << "Error: Table does not exist." << std::endl;
            return;
        }
        tables[tableName].insert(key, value);
        tables[tableName].saveToFile(tableName + ".csv");
    }

    void selectFrom(const std::string& tableName, const std::string& column1, const std::string& column2) {
        if (tables.find(tableName) == tables.end()) {
            std::cerr << "Error: Table does not exist." << std::endl;
            return;
        }

        std::cout << column1 << "\t" << column2 << std::endl;
        for (const auto& pair : tables[tableName].data) {
            std::cout << pair.first << "\t" << pair.second << std::endl;
        }
    }

    void deleteFrom(const std::string& tableName, const std::string& key) {
        if (tables.find(tableName) == tables.end()) {
            std::cerr << "Error: Table does not exist." << std::endl;
            return;
        }
        tables[tableName].deleteRow(key);
        tables[tableName].saveToFile(tableName + ".csv");
    }

    void loadSchema() {
        for (auto& pair : tables) {
            pair.second.loadFromFile(pair.first + ".csv");
        }
    }
};

#endif // DATABASE_HPP
