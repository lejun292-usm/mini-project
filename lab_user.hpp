#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#ifndef LAB_USER__HPP
#define LAB_USER__HPP

    void viewLabStock();
    void borrowLabItem(string matric);
    void returnLabItem(string matric);
    void searchLabItem();
    void viewBorrowedItem(string matric);
    string checkBorrowStatus(string matric);
    
#endif