#include <iostream>
#include <string>
#include <cstdlib> 
#include <iomanip>
#include <vector>
#include "lab_user.hpp"
#include "lab_admin.hpp"
#include "student.hpp"
#include "main.hpp"
#include <fstream>
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
        cout << "PLEASE SELECT AN OPTION:\n";
        
        // User Section (Available to all students) 
        cout << "[USER]\n";
        cout << "1. View Lab Stock\n";
        cout << "2. Search Lab Item\n";
        cout << "3. Borrow Lab Item\n";
        cout << "4. Return Lab Item\n";
        cout << "5. View Borrowed Item\n";
        cout << "\n==================================\n";
        
        // Admin Section (Password Protected inside functions) 
        cout << "[ADMIN]\n";
        cout << "6. Add New Item\n";
        cout << "7. View All Borrow Records\n";
        cout << "8. Delete Lab Item\n";
        cout << "9. Update Item Quantity\n";
        cout << "\n==================================\n";
        
        // Exit Section 
        cout << "[EXIT]\n";
        cout << "10. Logout\n";
        cout << "\n==================================\n";
        cout << "Select an option (1-10): ";
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
                searchLabItem(); 
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
                viewBorrowedItem(matric);
                cout << "\n"; 
                system("pause");
                cout << "\n";
                break;
            case 6: 
                system("cls");
                addLabItem();
                cout << "\n"; 
                system("pause");
                break;
            case 7: 
                system("cls");
                viewAllBorrowRecords(); 
                cout << "\n"; 
                system("pause");
                break;
            case 8: 
                system("cls");
                deleteLabItem(); 
                cout << "\n"; 
                system("pause");
                break;
            case 9:
                system("cls");
                updateItemQuantity(); 
                cout << "\n"; 
                system("pause");
                break;

            case 10: 
                system("cls");
                cout << "Logging out...\n"; 
                system("pause");
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
                system("pause");
                break;
        }

    } while (choice != 10); // Loop ends when 10 is selected
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
        cout << "PLEASE SELECT AN OPTION:\n";
        cout << "1. Login\n2. Register\n3. View Profile\n4. Exit\n";
        cout << "==========================\n";
        cout << "Select an option (1-4): ";
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

        // --- OPTION 2: REGISTER ---

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

        // --- OPTION 3: VIEW PROFILE ---

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

