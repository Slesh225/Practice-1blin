#ifndef TABLE_HPP
#define TABLE_HPP

#include "MyVector.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Table {
public:
    std::string name;
    MyVector<std::string> columns;
    MyVector<MyVector<std::string>> rows;

    Table() = default;

    Table(const std::string& name, const MyVector<std::string>& columns)
        : name(name), columns(columns) {}

    void insert(const MyVector<std::string>& values) {
        if (values.getSize() != columns.getSize()) {
            std::cerr << "Error: Number of values does not match number of columns." << std::endl;
            return;
        }
        rows.push_back(values);
    }

    void deleteRow(const MyVector<std::string>& conditions) {
        for (size_t i = 0; i < rows.getSize(); ++i) {
            bool match = true;
            for (size_t j = 0; j < conditions.getSize(); ++j) {
                if (rows[i][j] != conditions[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                for (size_t j = i; j < rows.getSize() - 1; ++j) {
                    rows[j] = rows[j + 1];
                }
                rows.pop_back();
                return;
            }
        }
        std::cerr << "Error: Row not found." << std::endl;
    }

    void print() const {
        for (size_t i = 0; i < columns.getSize(); ++i) {
            std::cout << columns[i] << "\t";
        }
        std::cout << std::endl;
        for (size_t i = 0; i < rows.getSize(); ++i) {
            for (size_t j = 0; j < rows[i].getSize(); ++j) {
                std::cout << rows[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }

    void saveToFile() const {
        std::ofstream outFile(name + ".csv");
        if (!outFile.is_open()) {
            std::cerr << "Error: Could not open file " << name << ".csv for writing." << std::endl;
            return;
        }

        for (size_t i = 0; i < columns.getSize(); ++i) {
            outFile << columns[i] << (i == columns.getSize() - 1 ? "\n" : ",");
        }
        for (size_t i = 0; i < rows.getSize(); ++i) {
            for (size_t j = 0; j < rows[i].getSize(); ++j) {
                outFile << rows[i][j] << (j == rows[i].getSize() - 1 ? "\n" : ",");
            }
        }
    }

    void loadFromFile() {
        std::ifstream inFile(name + ".csv");
        if (!inFile.is_open()) {
            std::cerr << "Error: Could not open file " << name << ".csv for reading." << std::endl;
            return;
        }

        std::string line;
        if (std::getline(inFile, line)) {
            std::istringstream ss(line);
            std::string value;
            while (std::getline(ss, value, ',')) {
                columns.push_back(value);
            }
        }

        while (std::getline(inFile, line)) {
            std::istringstream ss(line);
            MyVector<std::string> row;
            std::string value;
            while (std::getline(ss, value, ',')) {
                row.push_back(value);
            }
            rows.push_back(row);
        }
    }
};

#endif // TABLE_HPP
