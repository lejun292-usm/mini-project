#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> 
#include <vector>
#include <ctime>
#include "lab_user.hpp"

using namespace std;

// ( Data Structure )
// Defines what a "LabItem" looks like: it has a Name and a Quantity.

struct LabItem { 
    string name; 
    int quantity; 
};

// Function 1: Add New Item (Admin Only)
// Allows adding new equipment to the inventory file.

void addLabItem() {
    string itemName;
    int quantity;
    string password;

    // Security Check
    cout << "=== Admin Authentication Required ===\n";
    cout << "\nEnter Admin Password to Add Item: ";  
    cin >> password;
    if (password != "5555") {  
        cout << "[Error] Incorrect password.\n";
        return; // Stop function if password is wrong
    }
    system("cls");  // Clear screen

    cout << "[Access Granted].\n\n";
    cout << "\n==== Add New Lab Item ===\n";
    cout << "Enter Item Name (use_underscores_for_spaces): ";
    cin >> itemName; 
    cout << "Enter Quantity: ";
    cin >> quantity;

    // Open file in APPEND mode (ios::app) to add to the bottom

    ofstream stockFile("lab_items_stock.txt", ios::app);
    
    if (stockFile.is_open()) {
        stockFile << itemName << " " << quantity << endl;
        stockFile.close();
        cout << "[Success] Item added to inventory.\n";
    } else {
        cout << "Error: Unable to open stock file.\n";
    }
}

// Function 2: View All Borrow Records (Admin Only)

void viewAllBorrowRecords() {
    string password;
    cout << "=== Admin Authentication Required ===\n";
    cout << "Enter Admin Password: ";
    cin >> password;

    if (password != "5555") {
        cout << "[Access Denied] Wrong password.\n";
        return;
    }

    ifstream file("borrowed_data.txt");
    string matric, item;
    long long timeVal; // Variable to hold the time number

    cout << "\n================================================\n";
    cout << "            ALL BORROW RECORDS                  \n";
    cout << "================================================\n";
    cout << left << setw(15) << "Matric" << setw(25) << "Item" << "Time Code\n";
    cout << "------------------------------------------------\n";

    if (file.is_open()) {
        // Read 3 items : Matric, Item Name, and Time
        while (file >> matric >> item >> timeVal) {
            cout << left << setw(15) << matric << setw(25) << item << timeVal << endl;
        }
        file.close();
    } else {
        cout << "[Error] Could not open records file.\n";
    }
    cout << "================================================\n";
}

// Function 3 : Delete Item (Admin Only) 
void deleteLabItem() {
    string password;
    cout << "=== Admin Authentication Required ===\n";
    cout << "Enter Admin Password: ";
    cin >> password;

    if (password != "5555") {
        cout << "[Error] Incorrect password.\n";
        return;
    }

    // 1. Load all items into memory
    vector<LabItem> items;
    LabItem temp;
    ifstream in("lab_items_stock.txt");
    if (!in.is_open()) { cout << "Error opening file.\n"; return; }
    while (in >> temp.name >> temp.quantity) {
        items.push_back(temp);
    }
    in.close();

    if (items.empty()) {
        cout << "List is empty.\n";
        return;
    }

    // 2. Display List with Numbers
    system("cls");
    cout << "\n=== DELETE LAB ITEM ===\n";
    cout << left << setw(5) << "No." << setw(25) << "Item Name" << "Quantity" << endl;
    cout << "--------------------------------\n";
    for (int i = 0; i < items.size(); i++) {
        cout << left << setw(5) << (i + 1) << setw(25) << items[i].name << items[i].quantity << endl;
    }

    // 3. Ask for Number
    int choice;
    cout << "\nEnter Number to DELETE (0 to cancel): ";
    cin >> choice;

    if (choice == 0) return;
    if (choice < 1 || choice > items.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    // 4. Delete the item from memory (Vector)
    int index = choice - 1;
    string deletedName = items[index].name;
    
    // .erase() removes the item at that position
    items.erase(items.begin() + index);

    // 5. Save the new list back to file
    ofstream out("lab_items_stock.txt");
    for (auto i : items) {
        out << i.name << " " << i.quantity << endl;
    }
    out.close();

    cout << "[Success] Deleted item: " << deletedName << endl;
}

// Function 4 : Update Quantity (Restock)

void updateItemQuantity() {
    string password;
    cout << "=== Admin Authentication Required ===\n";
    cout << "Enter Admin Password: ";
    cin >> password;
    if (password != "5555") return;

    // Load items
    vector<LabItem> items;
    LabItem temp;
    ifstream in("lab_items_stock.txt");
    if (!in.is_open()) { cout << "Error opening file.\n"; return; }
    while (in >> temp.name >> temp.quantity) {
        items.push_back(temp);
    }
    in.close();

    if (items.empty()) {
        cout << "List is empty.\n";
        return;
    }

    // Display List
    system("cls");
    cout << "\n=== UPDATE ITEM QUANTITY ===\n";
    cout << left << setw(5) << "No." << setw(25) << "Item Name" << "Quantity" << endl;
    cout << "--------------------------------\n";
    for (int i = 0; i < items.size(); i++) {
        cout << left << setw(5) << (i + 1) << setw(25) << items[i].name << items[i].quantity << endl;
    }

    // Ask for Number
    int choice;
    cout << "\nEnter Number to UPDATE (0 to cancel): ";
    cin >> choice;

    if (choice == 0) return;
    if (choice < 1 || choice > items.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    // Ask for New Quantity
    int newQty;
    int index = choice - 1;
    cout << "Updating: " << items[index].name << "\n";
    cout << "Enter NEW Quantity: ";
    cin >> newQty;

    // 5. Update in memory
    items[index].quantity = newQty;

    // 6. Save back to file
    ofstream out("lab_items_stock.txt");
    for (auto i : items) {
        out << i.name << " " << i.quantity << endl;
    }
    out.close();

    cout << "[Success] Quantity updated for " << items[index].name << endl;
}

