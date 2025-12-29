#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "student.hpp"

using namespace std;

string searchStudent(string matric_to_find) {
    ifstream myfile("student.txt");
    string line;
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
        if (line.find(matric_to_find) != string::npos) {
            myfile.close();
            return line;
            }
        }
        myfile.close();
    }
        return "NOT_FOUND";
}

void registerStudent(string matric_num) {
    string name, email, ic;
    cout << "\n--- Registering New Student ---\n";
    cout << "Matric: " << matric_num << "\n";
    cin.ignore(); 
    cout << "Enter Name (Capital Letter): "; getline(cin, name);
    cout << "Enter IC (xxxxx-xx-xxxx): "; getline(cin, ic);
    cout << "Enter Email (xxx@student.usm.my): "; getline(cin, email);

    ofstream myfile("student.txt", ios::app);
    if (myfile.is_open()) {
        myfile << name << "\t" << matric_num << "\t" << ic << "\t" << email << "\n";
        myfile.close();
    }
}

string getNameFromLine(string line) {
    stringstream ss(line);
    string segment;
    if (getline(ss, segment, '\t')) 
    return segment; 
        return "Unknown";
}

void viewUserProfile(string matric_num) {
    string result = searchStudent(matric_num);
    
    if (result != "NOT_FOUND") {
        stringstream ss(result);
        string name, matric, ic, email;

        getline(ss, name, '\t');
        getline(ss, matric, '\t');
        getline(ss, ic, '\t');
        getline(ss, email, '\t');

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