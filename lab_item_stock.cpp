#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> 
#include <vector>
#include "lab_item_stock.hpp"

using namespace std;

struct LabItem {string name;int quantity;};

string checkBorrowStatus(string matric) {
    ifstream borrowFile("borrowed_data.txt");
    string fileMatric, fileItem;
    
    if (borrowFile.is_open()) {
        while (borrowFile >> fileMatric >> fileItem) {
            if (fileMatric == matric) {
                borrowFile.close();
                return fileItem; // Return the name of the item they have
            }
        }
        borrowFile.close();
    }
    return "NONE"; // They haven't borrowed anything
}

void viewLabStock() {  // View current lab item stock
    ifstream stockFile("lab_items_stock.txt");  // Open the stock file
    string itemName;
    int quantity;
    int id = 1;

    cout << "\n================================\n";
    cout << "      CURRENT LAB STOCK         \n";
    cout << "================================\n";

    if (!stockFile.is_open()) { // No stock file found
        cout << "System: No stock left \n";
        return;
    }
    // Stock File Found , Display items and quantities
    cout << left << setw(25) << "Item Name" << "Quantity" << endl;
    cout << "--------------------------------\n";

    while (stockFile >> itemName >> quantity) {
        cout << left << setw(5) << id << setw(25) << itemName << quantity << endl;
    }
    
    stockFile.close();
    cout << "================================\n";
}

void addLabItem() {
    string itemName;
    int quantity;
    string password;
    cout << "=== Admin Authentication Required ===\n";
    cout << "\nEnter Admin Password to Add Item: ";  // Enter password
    cin >> password;
    if (password != "5555") {  // Password Not Correct
        cout << "[Error] Incorrect password.\n";
        return;
    }
    system("cls");  

    // Password Correct , Proceed to add item
    cout << "[Access Granted].\n\n";
    cout << "\n==== Add New Lab Item ===\n";
    cout << "Enter Item Name (use_underscores_for_spaces): ";
    cin >> itemName; 
    cout << "Enter Quantity: ";
    cin >> quantity;

    ofstream stockFile("lab_items_stock.txt", ios::app);
    
    if (stockFile.is_open()) {
        stockFile << itemName << " " << quantity << endl;
        stockFile.close();
        cout << "[Success] Item added to inventory.\n";
    } else {
        cout << "Error: Unable to open stock file.\n";
    }
}



void borrowLabItem(string matric) {
    // A. CHECK IF ALREADY BORROWED
    string currentItem = checkBorrowStatus(matric);
    if (currentItem != "NONE") {
        cout << "\n[DENIED] You have already borrowed: " << currentItem << "\n";
        cout << "Please return it before borrowing another.\n";
        return;
    }

    // B. LOAD ITEMS
    vector<LabItem> allItems;
    LabItem temp;
    ifstream inFile("lab_items_stock.txt");
    if (!inFile.is_open()) { cout << "No stock file.\n"; return; }
    while (inFile >> temp.name >> temp.quantity) { allItems.push_back(temp); }
    inFile.close();

    // C. SHOW LIST
    system("cls");
    cout << "\n--- BORROW ITEM ---\n";
    cout << left << setw(5) << "No." << setw(25) << "Item Name" << "Quantity" << endl;
    for (int i = 0; i < allItems.size(); i++) {
        cout << left << setw(5) << (i + 1) << setw(25) << allItems[i].name << allItems[i].quantity << endl;
    }

    // D. CHOOSE ITEM
    int choice;
    cout << "\nEnter No. to borrow (0 to cancel): "; cin >> choice;
    if (choice == 0) return;
    if (choice < 1 || choice > allItems.size()) { cout << "Invalid choice.\n"; return; }

    int index = choice - 1;

    // E. UPDATE STOCK AND RECORD BORROW
    if (allItems[index].quantity > 0) {
        allItems[index].quantity--; 

        // 1. Update Stock File
        ofstream outFile("lab_items_stock.txt");
        for (int i = 0; i < allItems.size(); i++) {
            outFile << allItems[i].name << " " << allItems[i].quantity << endl;
        }
        outFile.close();

        // 2. Add to Borrowed Data File (Record the transaction)
        ofstream borrowOut("borrowed_data.txt", ios::app);
        borrowOut << matric << " " << allItems[index].name << endl;
        borrowOut.close();

        cout << "[Success] You borrowed: " << allItems[index].name << "\n";

    } else {
        cout << "[Error] Item is Out of Stock!\n";
    }
}

// 4. Return Item (BY NUMBER)
void returnLabItem(string matric) {
    // A. CHECK WHAT THEY BORROWED
    string borrowedItem = checkBorrowStatus(matric);
    
    if (borrowedItem == "NONE") {
        cout << "\n[Error] You have no active items to return.\n";
        return;
    }

    cout << "\nYou are returning: " << borrowedItem << "\n";
    cout << "Confirm Return? (1 = Yes, 0 = Cancel): ";
    int confirm;
    cin >> confirm;
    if (confirm != 1) return;

    // B. UPDATE STOCK (Increase Quantity)
    vector<LabItem> allItems;
    LabItem temp;
    ifstream inFile("lab_items_stock.txt");
    while (inFile >> temp.name >> temp.quantity) {
        if (temp.name == borrowedItem) {
            temp.quantity++; // Increase stock
        }
        allItems.push_back(temp);
    }
    inFile.close();

    ofstream outFile("lab_items_stock.txt");
    for (int i = 0; i < allItems.size(); i++) {
        outFile << allItems[i].name << " " << allItems[i].quantity << endl;
    }
    outFile.close();

    // C. REMOVE FROM BORROW FILE
    // We rewrite the file, but SKIP the line that belongs to this student
    vector<string> borrowLines;
    string fMatric, fItem;
    
    ifstream bIn("borrowed_data.txt");
    while (bIn >> fMatric >> fItem) {
        // Keep everyone else's data, skip THIS student's data
        if (fMatric != matric) {
            borrowLines.push_back(fMatric + " " + fItem);
        }
    }
    bIn.close();

    ofstream bOut("borrowed_data.txt");
    for (int i = 0; i < borrowLines.size(); i++) {
        bOut << borrowLines[i] << endl;
    }
    bOut.close();

    cout << "[Success] Item returned successfully. You can borrow again.\n";
}

void searchLabItem() {
    string keyword;
    cout << "Enter item name to search: ";
    cin >> keyword;

    ifstream file("lab_items_stock.txt");
    string name;
    int qty;
    bool found = false;

    cout << "\nSearch Result:\n";
    cout << left << setw(25) << "Item Name" << "Quantity\n";
    cout << "--------------------------------\n";

    while (file >> name >> qty) {
        if (name.find(keyword) != string::npos) {
            cout << left << setw(25) << name << qty << endl;
            found = true;
        }
    }

    if (!found)
        cout << "[Info] No matching item found.\n";

    file.close();
}
void deleteLabItem() {
    string password, delItem;
    cout << "Enter Admin Password: ";
    cin >> password;

    if (password != "5555") {
        cout << "[Error] Incorrect password.\n";
        return;
    }

    cout << "Enter item name to delete: ";
    cin >> delItem;

    vector<LabItem> items;
    LabItem temp;
    bool removed = false;

    ifstream in("lab_items_stock.txt");
    while (in >> temp.name >> temp.quantity) {
        if (temp.name != delItem)
            items.push_back(temp);
        else
            removed = true;
    }
    in.close();

    ofstream out("lab_items_stock.txt");
    for (auto i : items)
        out << i.name << " " << i.quantity << endl;
    out.close();

    if (removed)
        cout << "[Success] Item deleted.\n";
    else
        cout << "[Error] Item not found.\n";
}

void viewBorrowedItem(string matric) {
    string item = checkBorrowStatus(matric);

    if (item == "NONE")
        cout << "You have not borrowed any item.\n";
    else
        cout << "You have borrowed: " << item << endl;
}

void updateItemQuantity() {
    string password, itemName;
    int newQty;

    cout << "Enter Admin Password: ";
    cin >> password;
    if (password != "5555") return;

    cout << "Enter Item Name: ";
    cin >> itemName;
    cout << "Enter New Quantity: ";
    cin >> newQty;

    vector<LabItem> items;
    LabItem temp;
    bool updated = false;

    ifstream in("lab_items_stock.txt");
    while (in >> temp.name >> temp.quantity) {
        if (temp.name == itemName) {
            temp.quantity = newQty;
            updated = true;
        }
        items.push_back(temp);
    }
    in.close();

    ofstream out("lab_items_stock.txt");
    for (auto i : items)
        out << i.name << " " << i.quantity << endl;
    out.close();

    if (updated)
        cout << "[Success] Quantity updated.\n";
    else
        cout << "[Error] Item not found.\n";
}