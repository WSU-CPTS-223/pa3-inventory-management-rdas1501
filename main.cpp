#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include "Product.h"
#include "HashMap.h"
#include "CategoryMap.h"
#include "DynamicArray.h"

using namespace std;

// Global data structures
HashMap<string, Product>* inventoryById = nullptr;
CategoryMap* categoryMap = nullptr;

// Helper function to trim whitespace
string trim(const string& str) {
    size_t start = 0;
    size_t end = str.length();
    
    while (start < end && isspace(str[start])) start++;
    while (end > start && isspace(str[end - 1])) end--;
    
    return str.substr(start, end - start);
}

// Helper function to parse CSV line handling quoted fields
DynamicArray<string> parseCSVLine(const string& line) {
    DynamicArray<string> fields;
    string field;
    bool inQuotes = false;
    
    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];
        
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(trim(field));
    
    return fields;
}

// Load CSV data
bool loadCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }
    
    string line;
    bool firstLine = true;
    int lineCount = 0;
    
    while (getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue; // Skip header
        }
        
        if (line.empty()) continue;
        
        DynamicArray<string> fields = parseCSVLine(line);
        
        if (fields.getSize() >= 4) {
            string uniq_id = fields[0];
            string product_name = fields[1];
            string category = fields[2].empty() ? "NA" : fields[2];
            string price = fields[3];
            
            Product product(uniq_id, product_name, category, price);
            inventoryById->insert(uniq_id, product);
            categoryMap->addProduct(category, product);
            lineCount++;
        }
    }
    
    file.close();
    cout << "Loaded " << lineCount << " products from CSV." << endl;
    return true;
}

// Test functions using cassert
void testHashMapInsertFind() {
    HashMap<string, Product> testMap;
    Product p1("TEST001", "Test Product", "Test Category", "10.00");
    
    testMap.insert("TEST001", p1);
    
    Product found;
    bool result = testMap.find("TEST001", found);
    
    assert(result == true);
    assert(found.getId() == "TEST001");
    assert(found.getName() == "Test Product");
    
    cout << "✓ testHashMapInsertFind passed" << endl;
}

void testHashMapNotFound() {
    HashMap<string, Product> testMap;
    Product found;
    
    bool result = testMap.find("NONEXISTENT", found);
    assert(result == false);
    
    cout << "✓ testHashMapNotFound passed" << endl;
}

void testDynamicArray() {
    DynamicArray<int> arr;
    
    for (int i = 0; i < 20; i++) {
        arr.push_back(i);
    }
    
    assert(arr.getSize() == 20);
    assert(arr[0] == 0);
    assert(arr[19] == 19);
    
    cout << "✓ testDynamicArray passed" << endl;
}

void testCategoryMap() {
    CategoryMap catMap;
    Product p1("ID1", "Product1", "Electronics", "100");
    Product p2("ID2", "Product2", "Electronics", "200");
    Product p3("ID3", "Product3", "Books", "50");
    
    catMap.addProduct("Electronics", p1);
    catMap.addProduct("Electronics", p2);
    catMap.addProduct("Books", p3);
    
    DynamicArray<Product>* products = nullptr;
    bool found = catMap.findCategory("Electronics", products);
    
    assert(found == true);
    assert(products != nullptr);
    assert(products->getSize() == 2);
    
    cout << "✓ testCategoryMap passed" << endl;
}

void runAllTests() {
    cout << "\n=== Running Tests ===" << endl;
    testHashMapInsertFind();
    testHashMapNotFound();
    testDynamicArray();
    testCategoryMap();
    cout << "=== All Tests Passed ===\n" << endl;
}

void printHelp() {
    cout << "Supported list of commands: " << endl;
    cout << " 1. find <inventoryid> - Finds if the inventory exists. If exists, prints details. If not, prints 'Inventory not found'." << endl;
    cout << " 2. listInventory <category_string> - Lists just the id and name of all inventory belonging to the specified category. If the category doesn't exists, prints 'Invalid Category'.\n" << endl;
    cout << " Use :quit to quit the REPL" << endl;
}

bool validCommand(string line) {
    return (line == ":help") ||
           (line.rfind("find", 0) == 0) ||
           (line.rfind("listInventory", 0) == 0);
}

void evalCommand(string line) {
    if (line == ":help") {
        printHelp();
    }
    else if (line.rfind("find", 0) == 0) {
        // Extract inventory ID
        size_t spacePos = line.find(' ');
        if (spacePos == string::npos || spacePos + 1 >= line.length()) {
            cout << "Usage: find <inventoryid>" << endl;
            return;
        }
        
        string inventoryId = trim(line.substr(spacePos + 1));
        Product found;
        
        if (inventoryById->find(inventoryId, found)) {
            cout << "\nProduct Found:" << endl;
            found.display();
        } else {
            cout << "Inventory/Product not found" << endl;
        }
    }
    else if (line.rfind("listInventory", 0) == 0) {
        // Extract category
        size_t spacePos = line.find(' ');
        if (spacePos == string::npos || spacePos + 1 >= line.length()) {
            cout << "Usage: listInventory <category_string>" << endl;
            return;
        }
        
        string category = trim(line.substr(spacePos + 1));
        DynamicArray<Product>* products = nullptr;
        
        if (categoryMap->findCategory(category, products)) {
            if (products->getSize() == 0) {
                cout << "No products found in category: " << category << endl;
            } else {
                cout << "\nProducts in category '" << category << "':" << endl;
                cout << "----------------------------------------" << endl;
                for (int i = 0; i < products->getSize(); i++) {
                    Product p = (*products)[i];
                    cout << "ID: " << p.getId() << " | Name: " << p.getName() << endl;
                }
                cout << "Total: " << products->getSize() << " products" << endl;
            }
        } else {
            cout << "Invalid Category" << endl;
        }
    }
}

void bootStrap() {
    cout << "\n Welcome to Amazon Inventory Query System" << endl;
    cout << " enter :quit to exit. or :help to list supported commands." << endl;
    
    // Initialize data structures
    inventoryById = new HashMap<string, Product>(5000);
    categoryMap = new CategoryMap();
    
    // Run tests
    runAllTests();
    
    // Load CSV file
    string filename = "amazon-product-dataset-2020.csv";
    cout << "Loading inventory from " << filename << "..." << endl;
    
    if (!loadCSV(filename)) {
        cerr << "Warning: Could not load CSV file. System will run with empty inventory." << endl;
    }
    
    cout << "\n> ";
}

void cleanup() {
    delete inventoryById;
    delete categoryMap;
}

int main(int argc, char const *argv[]) {
    string line;
    bootStrap();
    
    while (getline(cin, line) && line != ":quit") {
        if (validCommand(line)) {
            evalCommand(line);
        } else {
            cout << "Command not supported. Enter :help for list of supported commands" << endl;
        }
        cout << "> ";
    }
    
    cleanup();
    cout << "Goodbye!" << endl;
    return 0;
}