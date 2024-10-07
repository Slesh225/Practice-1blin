#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP

#include <iostream>
#include <stdexcept>  // For std::out_of_range

template <typename T>
class MyVector {
private:
    T* data;
    size_t size;
    size_t capacity;

    void resize() {
        std::cout << "Resizing vector from capacity " << capacity << " to " << capacity * 2 << std::endl;
        capacity *= 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    MyVector() : size(0), capacity(1) {
        std::cout << "Creating vector with initial capacity " << capacity << std::endl;
        data = new T[capacity];
    }

    ~MyVector() {
        std::cout << "Destroying vector with capacity " << capacity << std::endl;
        delete[] data;
    }

    void push_back(const T& value) {
        std::cout << "Pushing back value " << value << " into vector with size " << size << " and capacity " << capacity << std::endl;
        if (size == capacity) {
            resize();
        }
        data[size++] = value;
    }

    void pop_back() {
        std::cout << "Popping back from vector with size " << size << std::endl;
        if (size > 0) {
            --size;
        }
    }

    size_t getSize() const {
        return size;
    }

    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Перегрузка оператора >>
    friend std::istream& operator>>(std::istream& is, MyVector<T>& vec) {
        T value;
        while (is >> value) {
            vec.push_back(value);
        }
        return is;
    }

    // Итераторы для начала и конца
    T* begin() {
        return data;
    }

    T* end() {
        return data + size;
    }

    // Const версия итераторов
    const T* begin() const {
        return data;
    }

    const T* end() const {
        return data + size;
    }

    // Перегрузка оператора ==
    bool operator==(const MyVector<T>& other) const {
        if (size != other.size) {
            return false;
        }
        for (size_t i = 0; i < size; ++i) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }
        return true;
    }
};

#endif // MYVECTOR_HPP
