#include <iostream>
#include <string>
#include <cstdlib> 
#include <iomanip>
#include <vector>
#include "lab_item_stock.hpp" 
#include "student.hpp"

using namespace std;

// =============================================================
// FUNCTION: showLabMenu
// PURPOSE: Displays the main dashboard after a student logs in.
// PARAMETERS: 
//    - studentName: Used to greet the user.
//    - matric: Passed to borrow/return functions to track who is doing what.
// =============================================================

void showLabMenu(string studentName , string matric) {
    int choice;
    
    // Do-While Loop: Keeps the menu open until user chooses "Logout"
    do {
        system("cls"); // Clears the previous screen for a clean UI
        
        // Header Section -- Welcome Message
        cout << "   WELCOME, " << studentName << "!\n";
        cout << "\n==================================\n";
        cout << "        LAB ITEM STOCK MENU      \n";
        cout << "==================================\n";
        cout << "Please select an option:\n";
        
        // User Section (Available to all students) 
        cout << "\n[USER]\n";
        cout << "1. View Lab Stock\n";
        cout << "2. Search Lab Item\n";
        cout << "3. Borrow Lab Item\n";
        cout << "4. Return Lab Item\n";
        cout << "\n==================================\n";
        
        // Admin Section (Password Protected inside functions) 
        cout << "\n[ADMIN]\n";
        cout << "5. Add New Item\n";
        cout << "6. View All Borrow Records\n";
        cout << "7. Delete Lab Item\n";
        cout << "8. Update Item Quantity\n";
        cout << "\n==================================\n";
        
        // Exit Section 
        cout << "\n[EXIT]\n";
        cout << "9. Logout\n";
        cout << "Enter Your Choice: ";
        cin >> choice;

        // --- INPUT VALIDATION (CRITICAL) ---
        // Checks if user typed a letter instead of a number.
        // If we don't fix this, the program will crash (infinite loop).
        if (cin.fail()) { 
            cin.clear(); // Reset the error flag
            cin.ignore(10000, '\n'); // Ignore the bad characters
            choice = 0; // Set invalid choice to trigger 'default' case
        }
      
        // Switch Case: Routes user to the correct function based on choice
        switch(choice) {
            case 1: 
                system("cls"); 
                viewLabStock(); // Call function from lab_item_stock.cpp
                cout << "\n"; 
                system("pause"); // Wait for user to read output
                break;
            case 2: 
                system("cls"); 
                searchLabItem(); // Call function from lab_item_stock.cpp
                cout << "\n"; 
                system("pause"); 
                break;
            case 3: 
                system("cls"); 
                borrowLabItem(matric); // Pass matric to record the transaction
                cout << "\n"; 
                system("pause");
                break;
            case 4: 
                system("cls"); 
                returnLabItem(matric); // Pass matric to find their loan
                cout << "\n"; 
                system("pause");
                break;
            case 5: 
                system("cls");
                addLabItem(); // Will ask for Admin Password "5555"
                cout << "\n";
                break;
            case 6: 
                system("cls");
                viewAllBorrowRecords(); // Call function from lab_item_stock.cpp
                cout << "\n"; 
                system("pause");
                break;
            case 7: 
                system("cls");
                deleteLabItem(); // Call function from lab_item_stock.cpp
                cout << "\n"; 
                system("pause");
                break;
            case 8: 
                system("cls");
                updateItemQuantity(); // Call function from lab_item_stock.cpp
                cout << "\n"; 
                system("pause");
                break;
            case 9: 
                system("cls");
                cout << "Logging out...\n"; 
                system("pause");
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                system("pause");
                break;
        }

    } while (choice != 9); // Loop ends when 9 is selected
}

// =============================================================
// FUNCTION: main
// PURPOSE: The Entry Point. Handles Login, Registration, and Exit.
// =============================================================

int main() {
    int loginchoice;
    string matricInput, result;

    // Main Login Loop
    do {
        system("cls");
        cout << "==========================\n";
        cout << "   STUDENT PORTAL LOGIN   \n";
        cout << "==========================\n";
        cout << "1. Login\n2. Register\n3. View Profile\n4. Exit\n";
        cout << "==========================\n";
        cout << "Enter Your Choice: ";
        cin >> loginchoice;

        // OPTION 1: LOGIN 

        if (loginchoice == 1) { 
            system("cls");
            cout << "\n[Login] Enter Your Matric Number: "; 
            cin >> matricInput;
            
            // Check student.txt database
            result = searchStudent(matricInput);
            
            if (result != "NOT_FOUND") {
                // If success, extract Name and Launch the Lab Menu
                showLabMenu(getNameFromLine(result), matricInput); 
            } else {
                cout << "Error: Not found.\n"; 
                system("pause");
            }
        }

        // OPTION 2: REGISTER 

        else if (loginchoice == 2) { 
            system("cls");
            cout << "\n[Register] Enter Your Matric Number: "; 
            cin >> matricInput;
            
            // Ensure student doesn't already exist
            if (searchStudent(matricInput) == "NOT_FOUND") {
                registerStudent(matricInput);
                cout << "Success! Please Login.\n";
            } else {
                cout << "Error: Already registered.\n";
            }
            system("pause");
        }

        // OPTION 3: VIEW PROFILE

        else if (loginchoice == 3) { 
            system("cls");
            cout << "\n[View Profile] Enter Your Matric Number: "; cin >> matricInput;
            viewUserProfile(matricInput); 
            system("pause");
        }
    } 
    while (loginchoice != 4); // Exit if 4 is selected

    cout << "Exiting program. Goodbye!\n";

    return 0;
}

