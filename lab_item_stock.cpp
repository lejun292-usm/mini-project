#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> 
#include <vector>
#include <ctime>
#include "lab_item_stock.hpp"

using namespace std;

// ( Data Structure )
// Defines what a "LabItem" looks like: it has a Name and a Quantity.

struct LabItem { string name; int quantity; };

// Function 1: Check Borrow Status 
// This function checks is there any borrowed item under the given matric number.
// It show the item name if found, or no item if none found.

string checkBorrowStatus(string matric) {
    ifstream borrowFile("borrowed_data.txt"); // Read borrowed data file
    string fileMatric, fileItem;
    long long fileTime; // Variable to hold the timestamp
    
    if (borrowFile.is_open()) {
        // Read line by line: Matric -> Item Name -> Time
        while (borrowFile >> fileMatric >> fileItem >> fileTime) {
            // If the matric number matches the student's matric
            if (fileMatric == matric) {
                borrowFile.close();
                return fileItem; // return the name of the item they have.
            }
        }
        borrowFile.close();
    }
    return "NONE"; // No match found, so they haven't borrowed anything.
}

// Function 2: View All Lab Stock
// Displays a table for all items currently in the text file.

void viewLabStock() {  
    ifstream stockFile("lab_items_stock.txt");  
    string itemName;
    int quantity;
    int id = 1; 

    cout << "\n================================\n";
    cout << "      CURRENT LAB STOCK         \n";
    cout << "================================\n";

    if (!stockFile.is_open()) { 
        cout << "System: No stock left \n";
        return;
    }
  
    // Print Table Headers
    // left = align text to the left
    // setw(25) = make this column 25 characters wide

    cout << left << setw(25) << "Item Name" << "Quantity" << endl;
    cout << "--------------------------------\n";

    // Loop through the file and print every item

    while (stockFile >> itemName >> quantity) {
        cout << left << setw(5) << id << setw(25) << itemName << quantity << endl;
        id++; // Increase index number (1, 2, 3...)
    }
    
    stockFile.close();
    cout << "================================\n";
}

// Function 3: Add New Item (Admin Only)
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

// Function 4: Borrow Item
// For students to borrow an item from the lab stock.

void borrowLabItem(string matric) {
   
    // Check if they already have an item

    string currentItem = checkBorrowStatus(matric);
    if (currentItem != "NONE") {
        cout << "\n[DENIED] You have already borrowed: " << currentItem << "\n";
        cout << "Please return it before borrowing another.\n";
        return;
    }

    // Load all items from file into memory using a vector

    vector<LabItem> allItems;
    LabItem temp;
    ifstream inFile("lab_items_stock.txt");
    if (!inFile.is_open()) { cout << "No stock file.\n"; return; }
    while (inFile >> temp.name >> temp.quantity) { allItems.push_back(temp); }
    inFile.close();

    // Display items for selection

    system("cls");
    cout << "\n=== BORROW ITEM ===\n";
    cout << left << setw(5) << "No." << setw(25) << "Item Name" << "Quantity" << endl;
    for (int i = 0; i < allItems.size(); i++) {
        cout << left << setw(5) << (i + 1) << setw(25) << allItems[i].name << allItems[i].quantity << endl;
    }

    // Get User Choice

    int choice;
    cout << "\nEnter No. to borrow (0 to cancel): "; cin >> choice;
    if (choice == 0) return;
    if (choice < 1 || choice > allItems.size()) { cout << "Invalid choice.\n"; return; }

    int index = choice - 1; // Convert 1-based choice to 0-based vector index

    // Step 5: Process Borrowing
    if (allItems[index].quantity > 0) {
        allItems[index].quantity--; // Reduce stock by 1

        // Update the Stock File
        ofstream outFile("lab_items_stock.txt");
        for (int i = 0; i < allItems.size(); i++) {
            outFile << allItems[i].name << " " << allItems[i].quantity << endl;
        }
        outFile.close();

        // Save to Borrowed Data File
        time_t now = time(0); // Get current timestamp (seconds since 1970)
        ofstream borrowOut("borrowed_data.txt", ios::app);
        
        // Save format: Matric ItemName Timestamp
        borrowOut << matric << " " << allItems[index].name << " " << now << endl; 
        borrowOut.close();

        cout << "[Success] You borrowed: " << allItems[index].name << "\n";
        cout << "Date/Time Recorded. Due in 3 days.\n";

    } else {
        cout << "[Error] Out of Stock!\n";
    }
}

// Return Item (With Fine Calculation)

void returnLabItem(string matric) {
    ifstream bIn("borrowed_data.txt");
    string fMatric, fItem;
    long long fTime; 
    long long borrowedTime = 0;
    string borrowedItemName = "NONE";
    bool found = false;

    //  Find what the user borrowed
    if (bIn.is_open()) {
        while (bIn >> fMatric >> fItem >> fTime) {
            if (fMatric == matric) {
                borrowedItemName = fItem;
                borrowedTime = fTime;
                found = true;
                break;
            }
        }
        bIn.close();
    }

    if (!found) {
        cout << "\n[Error] You have no items to return.\n";
        return;
    }

    // Calculate Overdue Fine
    time_t now = time(0); // Current time
    // Calculate difference in seconds
    double secondsDiff = difftime(now, (time_t)borrowedTime);
    // Convert seconds to days (60s * 60m * 24h)
    int daysBorrowed = secondsDiff / (60 * 60 * 24); 

    cout << "\n=== RETURN CHECK ===\n";
    cout << "Item: " << borrowedItemName << "\n";
    cout << "Days Held: " << daysBorrowed << " days\n";

    // If held longer than 3 days, charge RM 5 per extra day

    if (daysBorrowed > 3) {
        int overdueDays = daysBorrowed - 3;
        double fine = overdueDays * 5.00; 
        cout << "[OVERDUE] You are " << overdueDays << " days late!\n";
        cout << "Fine Amount: RM " << fixed << setprecision(2) << fine << "\n";
        cout << "Please pay the fine at the counter.\n";
    } else {
        cout << "[Status] Returned on time. No fine.\n";
    }

    // Confirmation
    cout << "\nConfirm Return? (1 = Yes, 0 = Cancel): ";
    int confirm;
    cin >> confirm;
    if (confirm != 1) return;

    // Increase Stock Quantity
    vector<LabItem> allItems;
    LabItem temp;
    ifstream inFile("lab_items_stock.txt");
    while (inFile >> temp.name >> temp.quantity) {
        if (temp.name == borrowedItemName) {
            temp.quantity++; // Add item back to stock
        }
        allItems.push_back(temp);
    }
    inFile.close();

    // Save updated stock
    ofstream outFile("lab_items_stock.txt");
    for (auto &item : allItems) {
        outFile << item.name << " " << item.quantity << endl;
    }
    outFile.close();

    // Remove User from Borrow List
    // Rewrite the file, but skip the line belonging to this user
    vector<string> borrowLines;
    ifstream bRead("borrowed_data.txt");
    while (bRead >> fMatric >> fItem >> fTime) {
        if (fMatric != matric) {
            // Keep everyone else's data
            borrowLines.push_back(fMatric + " " + fItem + " " + to_string(fTime));
        }
    }
    bRead.close();

    ofstream bOut("borrowed_data.txt");
    for (const string &line : borrowLines) {
        bOut << line << endl;
    }
    bOut.close();

    cout << "[Success] Item returned.\n";
}

// Search Item
void searchLabItem() {
    string keyword;
    cout << "\n=== Search Lab Item ===\n";
    cout << "Enter item name to search: ";
    cin >> keyword;

    ifstream file("lab_items_stock.txt");
    string name;
    int qty;
    bool found = false;
    cout << "\nSearch Result:\n";
    cout << left << setw(25) << "Item Name" << "Quantity\n";
    cout << "--------------------------------\n\n";

    // Scan file for keyword matches
    while (file >> name >> qty) {
        // string::npos means "not found", so != means it IS found
        if (name.find(keyword) != string::npos) {
            cout << left << setw(25) << name << qty << endl;
            found = true;
        }
    }

    if (!found)
        cout << "[Info] No matching item found.\n";

    file.close();
}

//  Delete Item (Admin Only) 

void deleteLabItem() {
    string password, delItem;
    cout << "=== Admin Authentication Required ===\n";
    cout << "Enter Admin Password: ";
    cin >> password;

    if (password != "5555") {
        cout << "[Error] Incorrect password.\n";
        return;
    }
    cout << "[Access Granted]\n";
    cout << "\n=== Delete Lab Item ===\n";
    cout << "Enter item name to delete ( Example : Beaker_500ml ): ";
    cin >> delItem;

    vector<LabItem> items;
    LabItem temp;
    bool removed = false;

    // Read all items EXCEPT the one we want to delete
    ifstream in("lab_items_stock.txt");
    while (in >> temp.name >> temp.quantity) {
        if (temp.name != delItem)
            items.push_back(temp);
        else
            removed = true;
    }
    in.close();

    // Write the remaining items back to the file
    ofstream out("lab_items_stock.txt");
    for (auto i : items)
        out << i.name << " " << i.quantity << endl;
    out.close();

    if (removed)
        cout << "[Success] Item deleted.\n";
    else
        cout << "[Error] Item not found.\n";
}

//  Check What You Borrowed 

void viewBorrowedItem(string matric) {
    string item = checkBorrowStatus(matric);

    if (item == "NONE")
        cout << "You have not borrowed any item.\n";
    else
        cout << "You have borrowed: " << item << endl;
}

// Update Quantity (Restock)

void updateItemQuantity() {
    string password, itemName;
    int newQty;
    cout << "=== Admin Authentication Required ===\n";
    cout << "Enter Admin Password: ";
    cin >> password;
    if (password != "5555") return;
    cout << "[Access Granted]\n";
    
    cout << "\n=== Update Item Quantity ===\n";
    cout << "Enter Item Name( Example : Beaker_500ml ): ";
    cin >> itemName;
    cout << "Enter New Quantity: ";
    cin >> newQty;

    vector<LabItem> items;
    LabItem temp;
    bool updated = false;

    // Read file and find the matching item
    ifstream in("lab_items_stock.txt");
    while (in >> temp.name >> temp.quantity) {
        if (temp.name == itemName) {
            temp.quantity = newQty; // Update the number
            updated = true;
        }
        items.push_back(temp);
    }
    in.close();

    // Save changes
    ofstream out("lab_items_stock.txt");
    for (auto i : items)
        out << i.name << " " << i.quantity << endl;
    out.close();

    if (updated)
        cout << "[Success] Quantity updated.\n";
    else
        cout << "[Error] Item not found.\n";
}

// View All Borrow Records (Admin Only)

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