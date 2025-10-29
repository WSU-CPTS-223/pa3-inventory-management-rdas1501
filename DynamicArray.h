#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <cassert>

template<typename T>
class DynamicArray {
private:
    T* arr;
    int capacity;
    int size;
    
    void resize() {
        capacity *= 2;
        T* newArr = new T[capacity];
        for (int i = 0; i < size; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }
    
public:
    DynamicArray(int cap = 10) : capacity(cap), size(0) {
        arr = new T[capacity];
    }
    
    ~DynamicArray() {
        delete[] arr;
    }
    
    void push_back(const T& value) {
        if (size >= capacity) {
            resize();
        }
        arr[size++] = value;
    }
    
    T& operator[](int index) {
        assert(index >= 0 && index < size);
        return arr[index];
    }
    
    const T& operator[](int index) const {
        assert(index >= 0 && index < size);
        return arr[index];
    }
    
    int getSize() const { return size; }
    
    bool isEmpty() const { return size == 0; }
};

#endif