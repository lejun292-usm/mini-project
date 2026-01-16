#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "student.hpp"

using namespace std;


/* Programmer : AMIR RIDZAN BIN AZLAN ( 25100166 )
=========================================================
 FUNCTION: searchStudent
 PURPOSE: Scans the database file to find if a student exists.
 It will return the full line of text if found, or "NOT_FOUND" if not.
 ========================================================= */

string searchStudent(string matric_to_find) {
    ifstream myfile("student.txt"); // Read from student.txt
    string line;
    
    // Check if the file opened successfully
    if (myfile.is_open()) {
        // Read the file line by line
        while (getline(myfile, line)) {
            // Check if the matric number exists in the current line
            // If matric number is found, return the entire line
            if (line.find(matric_to_find) != string::npos) {
                myfile.close(); // Close file to save memory
                return line;    // Return the full line containing student info
            }
        }
        myfile.close(); // Close file if loop finishes without finding anything
    }
    return "NOT_FOUND"; // Return specific signal if student is not in the file
}

/* Programmer : AMIR RIDZAN BIN AZLAN ( 25100166 )
=========================================================
// FUNCTION: registerStudent
// PURPOSE: Collects new user details and saves them to the file.
 ========================================================= */

void registerStudent(string matric_num) {
    string name, email, ic;
    
    cout << "\n--- Registering New Student ---\n";
    cout << "Matric: " << matric_num << "\n";
    
    // Clears the "Enter" key left in buffer from previous input
    cin.ignore(); 
    
    cout << "Enter Name (Capital Letter): "; 
    getline(cin, name); // Reads full name including spaces
    
    cout << "Enter IC (xxxxx-xx-xxxx): "; 
    getline(cin, ic);
    
    cout << "Enter Email (xxx@student.usm.my): "; 
    getline(cin, email);

    // Open file in append mode (ios::app) so we don't delete old data
    ofstream myfile("student.txt", ios::app);
    
    if (myfile.is_open()) {
        // Save data separated by tabs (\t) for easy reading later
        myfile << name << "\t" << matric_num << "\t" << ic << "\t" << email << "\n";
        myfile.close(); 
    }
}

// Extract just the Name from a data line

/* Programmer : AMIR RIDZAN BIN AZLAN ( 25100166 )
=========================================================
 FUNCTION: getNameFromLine
 PURPOSE: A helper utility to extract just the student's name 
          from a long string of data.
========================================================= */
string getNameFromLine(string line) {
    stringstream ss(line); // Turn the text line into a stream
    string segment;
    
    // Read characters until the first Tab (\t) is found
    // The first item in our file format is the Name

    if (getline(ss, segment, '\t')) 
        return segment; // Return the name part
        
    return "Unknown"; 
}

/* LIM YU HAN ( 25100159 )
 =========================================================
 FUNCTION: viewUserProfile
 PURPOSE: Finds a student and displays their info in a formatted table.
========================================================= */

void viewUserProfile(string matric_num) {
    // reusing the search function to get the data string
    string result = searchStudent(matric_num);
    
    if (result != "NOT_FOUND") {
        stringstream ss(result); // Create a stream from the result string
        string name, matric, ic, email;

        // Break the line apart using Tab (\t) as the separator
        getline(ss, name, '\t');   // 1st part (Name)
        getline(ss, matric, '\t'); // 2nd part (Matric)
        getline(ss, ic, '\t');     // 3rd part (IC)
        getline(ss, email, '\t');  // 4th part (Email)

        // Print the parsed data neatly
        cout << "\n===============================\n";
        cout << "       STUDENT PROFILE         \n";
        cout << "===============================\n";
        cout << "Name   : " << name << endl;
        cout << "Matric : " << matric << endl;
        cout << "IC No  : " << ic << endl;
        cout << "Email  : " << email << endl;
        cout << "===============================\n";
    } else {
        cout << "\n[Error] Matric number not found.\n";
    }
}
