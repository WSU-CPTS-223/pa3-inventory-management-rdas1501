#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <iostream>
using namespace std;

class Product {
private:
    string uniq_id;
    string product_name;
    string category;
    string price;
    
public:
    // Default constructor
    Product() : uniq_id(""), product_name(""), category(""), price("") {}
    
    // Parameterized constructor
    Product(string id, string name, string cat, string pr) 
        : uniq_id(id), product_name(name), category(cat), price(pr) {}
    
    // Getters
    string getId() const { return uniq_id; }
    string getName() const { return product_name; }
    string getCategory() const { return category; }
    string getPrice() const { return price; }
    
    // Display product details
    void display() const {
        cout << "ID: " << uniq_id << endl;
        cout << "Name: " << product_name << endl;
        cout << "Category: " << category << endl;
        cout << "Price: " << price << endl;
    }
    
    // Check if product is valid (has an ID)
    bool isValid() const {
        return !uniq_id.empty();
    }
};

#endif