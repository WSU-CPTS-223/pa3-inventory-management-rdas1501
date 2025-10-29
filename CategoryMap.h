#ifndef CATEGORYMAP_H
#define CATEGORYMAP_H

#include "Product.h"
#include "DynamicArray.h"
#include <string>
using namespace std;

class CategoryMap {
private:
    struct CategoryNode {
        string category;
        DynamicArray<Product> products;
        CategoryNode* next;
        
        CategoryNode(string cat) : category(cat), next(nullptr) {}
    };
    
    CategoryNode* head;
    
public:
    CategoryMap() : head(nullptr) {}
    
    ~CategoryMap() {
        CategoryNode* current = head;
        while (current != nullptr) {
            CategoryNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    
    void addProduct(const string& category, const Product& product) {
        // Find or create category node
        CategoryNode* current = head;
        CategoryNode* prev = nullptr;
        
        while (current != nullptr) {
            if (current->category == category) {
                current->products.push_back(product);
                return;
            }
            prev = current;
            current = current->next;
        }
        
        // Category doesn't exist, create new node
        CategoryNode* newNode = new CategoryNode(category);
        newNode->products.push_back(product);
        
        if (prev == nullptr) {
            head = newNode;
        } else {
            prev->next = newNode;
        }
    }
    
    bool findCategory(const string& category, DynamicArray<Product>*& products) {
        CategoryNode* current = head;
        
        while (current != nullptr) {
            if (current->category == category) {
                products = &(current->products);
                return true;
            }
            current = current->next;
        }
        return false;
    }
};

#endif