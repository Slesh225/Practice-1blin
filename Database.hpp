#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <fstream>
#include <iostream>
#include <string>
#include "Table.hpp"
#include "MyMap.hpp" // Самописный MyMap
#include "MyVector.hpp" // Самописный MyVector
#include "json.hpp" // Для работы с JSON (используйте nlohmann/json)

using json = nlohmann::json; // Определение для использования JSON

class Database {
public:
    Database() {
        loadSchema();  // Загружаем схему из файла schema.json при инициализации базы данных
    }

    void createTable(const std::string& name, const MyVector<std::string>& columns) {
        if (tables.find(name)) {
            std::cerr << "Error: Table already exists." << std::endl;
            return;
        }
        tables.insert(name, Table(name, columns));
        saveSchema();  // Сохраняем обновленную схему
    }

    void insertInto(const std::string& tableName, const MyVector<std::string>& values) {
        if (!tables.find(tableName)) {
            std::cerr << "Error: Table does not exist." << std::endl;
            return;
        }
        tables[tableName].insert(values);
        tables[tableName].saveToFile();  // Сохраняем обновленные данные в CSV
    }

    void deleteFrom(const std::string& tableName, const MyVector<std::string>& values) {
        if (!tables.find(tableName)) {
            std::cerr << "Error: Table does not exist." << std::endl;
            return;
        }
        tables[tableName].deleteRow(values);
        tables[tableName].saveToFile();  // Обновляем CSV после удаления
    }

    void selectFrom(const std::string& tableName) {
        if (!tables.find(tableName)) {
            std::cerr << "Error: Table does not exist." << std::endl;
            return;
        }
        tables[tableName].print();  // Выводим содержимое таблицы
    }

private:
    MyMap<std::string, Table> tables;  // MyMap для хранения таблиц по именам

    void loadSchema() {
        std::ifstream inFile("schema.json");  // Открываем файл schema.json
        if (!inFile.is_open()) {
            std::cerr << "Error: Could not open schema.json" << std::endl;
            return;
        }

        json schema;
        inFile >> schema;  // Читаем JSON из файла
        for (const auto& item : schema["tables"].items()) {
            std::string tableName = item.key();
            MyVector<std::string> columns;
            for (const auto& column : item.value()["columns"]) {
                columns.push_back(column.get<std::string>());
            }
            tables.insert(tableName, Table(tableName, columns));  // Восстанавливаем таблицу
            tables[tableName].loadFromFile();  // Загружаем данные из CSV
        }
    }

    void saveSchema() {
        json schema;
        for (const auto& name : tables.keys()) {
            json columnsJson;
            for (const auto& column : tables[name].columns) {
                columnsJson.push_back(column);
            }
            schema["tables"][name]["columns"] = columnsJson;  // Сохраняем колонки таблиц
        }

        std::ofstream outFile("schema.json");  // Записываем схему обратно в schema.json
        if (outFile.is_open()) {
            outFile << schema.dump(4);  // Форматируем JSON с отступами
        }
        else {
            std::cerr << "Error: Could not save schema.json" << std::endl;
        }
    }
};

#endif // DATABASE_HPP