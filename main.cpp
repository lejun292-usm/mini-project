/* Mini Project Title : LAB EQUIPMENT STATUS RECORDER SYSTEM
   Purpose : The purpose of the system is to digitally manage and track laboratory equipment,
             allowing administrators to maintain accurate stock levels while enabling students 
             to borrow and return items with an automated mechanism for calculating fines on overdue returns.
             
    Programmer Name and Matric Number : AMIR RIDZAN BIN AZLAN ( 25100166 ) ------- Leader
                                        TAN LE JUN ( 24304103 )
                                        LIM YU HAN ( 25100159 )
                                        THANISHWARAN A/L M.SARAVANAN ( 24304965 )
                                        LEE PUEH KANG ( 24302612 )
                                        


             
             */





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


/* Programmer : TAN LE JUN ( 24304103 )
=============================================================
FUNCTION: showLabMenu
PURPOSE: Displays the main dashboard after a student logs in.
// ============================================================= */
    

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
            cin.clear(); // 1. Clear the error flag on cin so it works again
            cin.ignore(10000, '\n'); // 2. Discard the invalid input from the buffer
            choice = 0; // 3. Set choice to 0 so it hits the 'default' case below
        }
      
        // --- ROUTING BLOCK (Switch Statement) ---
        // Based on the integer value of 'choice', we execute specific code blocks.
        switch(choice) {
            
            // CASE 1: VIEW STOCK
            case 1: 
                system("cls"); // Clear menu
                viewLabStock(); // Call external function to show list
                cout << "\n"; 
                system("pause"); // Wait for user to press a key before going back to loop
                break; // Exit switch and go to 'while' condition
            
            // CASE 2: SEARCH
            case 2: 
                system("cls"); 
                searchLabItem(); 
                cout << "\n"; 
                system("pause"); 
                break;
            
            // CASE 3: BORROW
            case 3: 
                system("cls"); 
                // We pass 'matric' here so the system knows WHO is borrowing the item.
                borrowLabItem(matric); 
                cout << "\n"; 
                system("pause");
                break;
            
            // CASE 4: RETURN
            case 4: 
                system("cls"); 
                // We pass 'matric' to find items linked to this specific student.
                returnLabItem(matric); 
                cout << "\n"; 
                system("pause");
                break;
            
            // CASE 5: VIEW OWN LOANS
            case 5: 
                system("cls");
                viewBorrowedItem(matric);
                cout << "\n"; 
                system("pause");
                cout << "\n";
                break;
            
            // CASE 6: ADD ITEM (Admin)
            case 6: 
                system("cls");
                addLabItem(); // Calls function to create new stock
                cout << "\n"; 
                system("pause");
                break;
            
            // CASE 7: VIEW ALL RECORDS (Admin)
            case 7: 
                system("cls");
                viewAllBorrowRecords(); 
                cout << "\n"; 
                system("pause");
                break;
            
            // CASE 8: DELETE ITEM (Admin)
            case 8: 
                system("cls");
                deleteLabItem(); 
                cout << "\n"; 
                system("pause");
                break;
            
            // CASE 9: UPDATE QUANTITY (Admin)
            case 9:
                system("cls");
                updateItemQuantity(); 
                cout << "\n"; 
                system("pause");
                break;

            // CASE 10: LOGOUT
            case 10: 
                system("cls");
                cout << "Logging out...\n"; 
                system("pause");
                // The break happens, then the 'while' condition checks 'choice'.
                // Since choice is 10, the loop will terminate.
                break;

            // DEFAULT: ERROR HANDLING
            // This runs if the user types a number not between 1-10
            default:
                cout << "Invalid choice. Please try again.\n";
                system("pause");
                break;
        }

    } while (choice != 10); // Loop ends when 10 is selected
}

/* Programmer : TAN LE JUN ( 24304103 )
=============================================================
 FUNCTION: main
 PURPOSE: The Entry Point. Handles Login, Registration, and Exit.
============================================================= */

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


