#ifndef HASHMAP_H
#define HASHMAP_H

#include "Product.h"
#include <cassert>

// Simple hash map implementation using chaining
template<typename K, typename V>
class HashMap {
private:
    struct Node {
        K key;
        V value;
        Node* next;
        
        Node(K k, V v) : key(k), value(v), next(nullptr) {}
    };
    
    Node** table;
    int capacity;
    int size;
    
    // Simple hash function
    int hash(const string& key) const {
        unsigned long h = 0;
        for (char c : key) {
            h = h * 31 + c;
        }
        return h % capacity;
    }
    
public:
    HashMap(int cap = 1000) : capacity(cap), size(0) {
        table = new Node*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }
    
    ~HashMap() {
        for (int i = 0; i < capacity; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }
    
    void insert(const K& key, const V& value) {
        int index = hash(key);
        Node* current = table[index];
        
        // Check if key already exists
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        
        // Insert new node at beginning
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        size++;
    }
    
    bool find(const K& key, V& value) const {
        int index = hash(key);
        Node* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                value = current->value;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    int getSize() const { return size; }
};

#endif