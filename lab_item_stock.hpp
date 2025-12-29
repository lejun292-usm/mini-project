#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#ifndef LAB_ITEM_STOCK_HPP
#define LAB_ITEM_STOCK_HPP

    void viewLabStock();
    void addLabItem();
    void showLabMenu(string studentName);
    void borrowLabItem(string matric);
    void returnLabItem(string matric);
    void searchLabItem();
    void deleteLabItem();
    void viewBorrowedItem(string matric);
    void viewAllBorrowRecords();
    void updateItemQuantity();

#endif