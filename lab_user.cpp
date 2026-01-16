#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> 
#include <vector>
#include <ctime>
#include "lab_user.hpp"

using namespace std;

/* LIM YU HAN ( 25100159 )
=========================================================
 DATA STRUCTURE: LabItem
 PURPOSE: A blueprint for a single item in our stock. 
 Instead of managing two separate arrays (one for names, one for qty),
 we group them into a single object.
========================================================= */   

struct LabItem { 
    string name; 
    int quantity; 
};

/* LIM YU HAN ( 25100159 )
=========================================================
 Function 1: Check Borrow Status 
 This function checks is there any borrowed item under the given matric number.
 It show the item name if found, or no item if none found.
=========================================================*/
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

/* LIM YU HAN ( 25100159 )
=========================================================
 FUNCTION: viewLabStock
 PURPOSE: Displays a neatly formatted table of all items 
          available in 'lab_items_stock.txt'.
========================================================= */

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

/* THANISHWARAN A/L M.SARAVANAN ( 24304965 )
=========================================================
 FUNCTION: returnLabItem
 PURPOSE: Handles the complex logic of returning an item:
          1. Find the loan record.
          2. Calculate late fines (if any).
          3. Add the item back to stock (+1).
          4. Remove the loan record from the file.
========================================================= */
// PARAMETERS: matric (The student's matric number)

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

    // If no borrowed item found, exit function

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

/* THANISHWARAN A/L M.SARAVANAN ( 24304965 )
=========================================================
 FUNCTION: searchLabItem
 PURPOSE: Allows users to search for items by typing part of the name.
========================================================= */

void searchLabItem() {
    string keyword;
    cout << "\n=== Search Lab Item ===\n";
    cout << "Enter item name to search: ";
    cin >> keyword;

    // Open stock file and search for matches

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

    // If no matches found, inform the user

    if (!found)
        cout << "[Info] No matching item found.\n";

    file.close();
}

/* THANISHWARAN A/L M.SARAVANAN ( 24304965 )
=========================================================
 FUNCTION: viewBorrowedItem
 PURPOSE: Displays the item currently borrowed by a student.
========================================================= */

void viewBorrowedItem(string matric) {
    string item = checkBorrowStatus(matric);

    // Display borrowed item

    cout << "\n=== YOUR BORROWED ITEM ===\n";
    if (item == "NONE")
        cout << "You have not borrowed any item.\n";
    else
        cout << "You have borrowed: " << item << endl;
}

/* THANISHWARAN A/L M.SARAVANAN ( 24304965 )
=========================================================
 FUNCTION: borrowLabItem
 PURPOSE: Allows a student to borrow an item if available.
========================================================= */

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

    // Process Borrowing
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

