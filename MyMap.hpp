#ifndef MYMAP_HPP
#define MYMAP_HPP

#include <iostream>
#include <string>
#include <functional>  // For std::hash
#include "MyVector.hpp"

template <typename K, typename V>
class MyMap {
private:
    struct Node {
        K key;
        V value;
        Node* next;

        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    Node** table;
    size_t capacity;

    size_t hash(const K& key) const {
        return std::hash<K>()(key) % capacity;
    }

public:
    MyMap(size_t cap = 101) : capacity(cap) {
        table = new Node * [capacity] { nullptr };
    }

    ~MyMap() {
        for (size_t i = 0; i < capacity; ++i) {
            Node* entry = table[i];
            while (entry) {
                Node* temp = entry;
                entry = entry->next;
                delete temp;
            }
        }
        delete[] table;
    }

    Node* find(const K& key) const {
        size_t idx = hash(key);
        Node* entry = table[idx];
        while (entry) {
            if (entry->key == key) {
                return entry;
            }
            entry = entry->next;
        }
        return nullptr;
    }

    void insert(const K& key, const V& value) {
        size_t idx = hash(key);
        Node* entry = table[idx];

        // Если ключ уже существует, обновляем значение
        while (entry) {
            if (entry->key == key) {
                entry->value = value;
                return;
            }
            entry = entry->next;
        }

        // Если ключ не найден, создаем новую запись
        Node* newNode = new Node(key, value);
        newNode->next = table[idx];
        table[idx] = newNode;
    }

    // Оператор [] для доступа к элементу
    V& operator[](const K& key) {
        size_t idx = hash(key);
        Node* entry = table[idx];

        // Ищем существующий ключ
        while (entry) {
            if (entry->key == key) {
                return entry->value;  // Возвращаем существующее значение
            }
            entry = entry->next;
        }

        // Если ключ не найден, создаем новый элемент с значением по умолчанию
        V defaultValue;
        insert(key, defaultValue);
        return table[idx]->value;  // Возвращаем новое значение
    }

    // Метод для получения всех ключей
    MyVector<K> keys() const {
        MyVector<K> result;
        for (size_t i = 0; i < capacity; ++i) {
            Node* entry = table[i];
            while (entry) {
                result.push_back(entry->key);
                entry = entry->next;
            }
        }
        return result;
    }

    // Итераторы для начала и конца
    Node** begin() {
        return table;
    }

    Node** end() {
        return table + capacity;
    }

    // Const версия итераторов
    const Node* const* begin() const {
        return table;
    }

    const Node* const* end() const {
        return table + capacity;
    }
};

#endif // MYMAP_HPP