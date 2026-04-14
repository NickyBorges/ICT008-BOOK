#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>
#include "Header-classes-library.h"

using namespace std;


// ==================================================
//                  MAIN PROGRAM
// ==================================================
int main_2() {
    Library library;

    Author a1(1, "Rick Riordan", "American");
    Author a2(2, "Suzanne Collins", "American");
    Author a3(3, "C.S. Lewis", "British");
    Author a4(4, "Cassandra Clare", "American");
    Author a5(5, "Naomi Novik", "American");

    library.addBook(0, new HardcopyBook("The Lightning Thief", a1, "1111", true, "2005", "A-10"));
    library.addBook(1, new HardcopyBook("The Hunger Games", a2, "2222", true, "2008", "B-07"));
    library.addBook(2, new HardcopyBook("The Lion, the Witch and the Wardrobe", a3, "3333", true, "1950", "C-01"));
    library.addBook(3, new HardcopyBook("City of Bones", a4, "4444", true, "2007", "D-05"));
    library.addBook(4, new EBook("A Deadly Education", a5, "5555", true, "2020", "2028-12-31"));

    library.sortBooks();

    int choice;
    string input;

    while (true) {
        cout << "\n===== LIBRARY MENU =====" << endl;
        cout << "1. Borrow a Book" << endl;
        cout << "2. See Available Books" << endl;
        cout << "3. Return a Book" << endl;
        cout << "4. See Borrowed Books" << endl;
        cout << "5. See All Books" << endl;
        cout << "6. Exit Program" << endl;
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore();

        if (choice == 6) {
            cout << "Program terminated." << endl;
            break;
        }

        if (choice == 1) {
            cout << "\nEnter ISBN or Title (or 0 to cancel): ";
            getline(cin, input);

            if (input == "0") continue;

            library.borrowBookByISBNOrTitle(input);
            continue;
        }

        if (choice == 2) {
            library.displayAvailableBooks();
            continue;
        }

        if (choice == 3) {
            cout << "\nEnter ISBN to return: ";
            getline(cin, input);
            library.returnBookByISBN(input);
            continue;
        }

        if (choice == 4) {
            library.displayBorrowedBooks();
            continue;
        }

        if (choice == 5) {
            library.displayAllBooks();
            continue;
        }

        cout << "Invalid option.\n";
    }

    return 0;
}
