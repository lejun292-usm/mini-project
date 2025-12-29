#include <iostream>
#include <string>
#include <cstdlib> 
#include <iomanip>
#include <vector>
#include "lab_item_stock.hpp" 
#include "student.hpp"

using namespace std;

void showLabMenu(string studentName , string matric) {
    int choice;
    do {
        system("cls"); 
        cout << "   WELCOME, " << studentName << "!\n";
        cout << "\n==================================\n";
        cout << "        LAB ITEM STOCK MENU      \n";
        cout << "==================================\n";
        cout << "Please select an option:\n";
        cout << "\n[USER]\n";
        cout << "1. View Lab Stock\n";
        cout << "2. Search Lab Item\n";
        cout << "3. Borrow Lab Item\n";
        cout << "4. Return Lab Item\n";

        cout << "\n[ADMIN]\n";
        cout << "5. Add New Item\n";
        cout << "6. View All Borrow Records\n";
        cout << "7. Delete Lab Item\n";
        cout << "8. Update Item Quantity\n";
        cout << "9. Logout\n";
        cout << "Enter Your Choice: ";
        cin >> choice;

        if (cin.fail()) { 
            cin.clear(); // 1. Clear error flag
            cin.ignore(10000, '\n'); // 2. Discard bad input
            choice = 0; // 3. Set to invalid number so it goes to default case
        }
        // --- INPUT HANDLING END ---

        switch(choice) {
            case 1: 
                system("cls"); 
                viewLabStock(); 
                cout << "\n"; 
                system("pause"); 
                break;
            case 2: 
                system("cls"); 
                searchLabItem();
                cout << "\n"; 
                system("pause"); 
                break;
            case 3: 
                system("cls"); 
                borrowLabItem(matric);
                cout << "\n"; 
                system("pause");
                break;
            case 4: 
                system("cls"); 
                returnLabItem(matric);
                cout << "\n"; 
                system("pause");
                break;
            case 5: 
                system("cls");
                break;
            case 6: 
                system("cls");
                viewAllBorrowRecords();
                cout << "\n"; 
                system("pause");
                break;
            case 7: 
                system("cls");
                deleteLabItem();
                cout << "\n"; 
                system("pause");
                break;
            case 8: 
                system("cls");
                updateItemQuantity();
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

    } 

    while (choice !=9);
}

int main() {
    int loginchoice;
    string matricInput, result;

    do {
        system("cls");
        cout << "==========================\n";
        cout << "   STUDENT PORTAL LOGIN   \n";
        cout << "==========================\n";
        cout << "1. Login\n2. Register\n3. View Profile\n4. Exit\n";
        cout << "Enter choice ( 1 / 2 / 3 / 4 ): ";
        cin >> loginchoice;

        if (loginchoice == 1) { // LOGIN
            system("cls");
            cout << "\n[Login] Enter Your Matric Number: "; 
            cin >> matricInput;
            result = searchStudent(matricInput);
            
            if (result != "NOT_FOUND") {
                showLabMenu(getNameFromLine(result), matricInput); 
            } else {
                cout << "Error: Not found.\n"; system("pause");
            }
        }
        else if (loginchoice == 2) { // REGISTER
            system("cls");
            cout << "\n[Register] Enter Your Matric Number: "; 
            cin >> matricInput;
            if (searchStudent(matricInput) == "NOT_FOUND") {
                registerStudent(matricInput);
                cout << "Success! Please Login.\n";
            } else {
                cout << "Error: Already registered.\n";
            }
            system("pause");
        }
        else if (loginchoice == 3) { // VIEW PROFILE
            system("cls");
            cout << "\n[View Profile] Enter Your Matric Number: "; cin >> matricInput;
            viewUserProfile(matricInput); 
            system("pause");
        }
    } 
        while (loginchoice != 4); // EXIT
            cout << "Exiting program. Goodbye!\n";

    return 0;
}

void viewAllBorrowRecords() {
    string password;
    cout << "Enter Admin Password: ";
    cin >> password;

    if (password != "5555") {
        cout << "[Access Denied]\n";
        return;
    }

    ifstream file("borrowed_data.txt");
    string matric, item;

    cout << "\nBorrowed Records\n";
    cout << left << setw(15) << "Matric" << "Item\n";
    cout << "-----------------------------\n";

    while (file >> matric >> item) {
        cout << left << setw(15) << matric << item << endl;
    }
    file.close();
}