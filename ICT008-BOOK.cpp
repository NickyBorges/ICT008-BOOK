#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>
using namespace std;

// ==================================================
//                BASE CLASS: BOOK
// ==================================================
class Book {
protected:
    string title;
    string author;
    string isbn;
    bool availability;
    string dateAdd;

    bool isBorrowed;
    chrono::system_clock::time_point borrowDate;
    chrono::system_clock::time_point dueDate;

public:
    Book(string t, string a, string i, bool avail, string date)
        : title(t), author(a), isbn(i), availability(avail),
        dateAdd(date), isBorrowed(false) {
    }

    virtual ~Book() {}

    virtual void displayBookDetails() {
        cout << "----------------------------------" << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << isbn << endl;
        cout << "Availability: " << (availability ? "Available" : "Borrowed") << endl;
        cout << "Date Added: " << dateAdd << endl;

        if (!availability) {
            time_t due = chrono::system_clock::to_time_t(dueDate);
            cout << "Due Date: " << ctime(&due);
        }
        cout << "----------------------------------" << endl;
    }

    // SAME BORROW LOGIC AS PHASE 1
    bool borrowBook() {
        if (availability) {
            availability = false;
            isBorrowed = true;

            borrowDate = chrono::system_clock::now();
            dueDate = borrowDate + chrono::hours(24 * 10);

            time_t raw = chrono::system_clock::to_time_t(dueDate);
            tm* d = localtime(&raw);

            if (d->tm_wday == 6) {
                dueDate += chrono::hours(48);
            }
            else if (d->tm_wday == 0) {
                dueDate += chrono::hours(24);
            }

            return true;
        }
        return false;
    }

    bool returnBook(double& fineOut) {
        if (!availability) {
            availability = true;
            isBorrowed = false;

            auto now = chrono::system_clock::now();

            if (now > dueDate) {
                auto lateHours = chrono::duration_cast<chrono::hours>(now - dueDate).count();
                int lateDays = lateHours / 24;
                fineOut = lateDays * 0.30;
            }
            else {
                fineOut = 0.0;
            }
            return true;
        }
        return false;
    }

    string getISBN() { return isbn; }
    string getTitle() { return title; }
    bool isAvailable() { return availability; }

    static void sortBookData(Book* books[], int size) {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (books[j]->isbn > books[j + 1]->isbn) {
                    Book* temp = books[j];
                    books[j] = books[j + 1];
                    books[j + 1] = temp;
                }
            }
        }
    }
};

// ==================================================
//           DERIVED CLASS: HARDCOPY BOOK
// ==================================================
class HardcopyBook : public Book {
private:
    string shelfNumber;

public:
    HardcopyBook(string t, string a, string i, bool avail, string date, string shelf)
        : Book(t, a, i, avail, date), shelfNumber(shelf) {
    }

    void displayBookDetails() override {
        Book::displayBookDetails();
        cout << "Shelf Number: " << shelfNumber << endl;
        cout << "----------------------------------" << endl;
    }
};

// ==================================================
//             DERIVED CLASS: EBOOK
// ==================================================
class EBook : public Book {
private:
    string licenseEndDate;

public:
    EBook(string t, string a, string i, bool avail, string date, string licenseEnd)
        : Book(t, a, i, avail, date), licenseEndDate(licenseEnd) {
    }

    void displayBookDetails() override {
        Book::displayBookDetails();
        cout << "License End Date: " << licenseEndDate << endl;
        cout << "----------------------------------" << endl;
    }
};

// ==================================================
//                  MAIN PROGRAM
// ==================================================
int main() {
    const int SIZE = 5;
    Book* books[SIZE];

    // Create HARD COPY and EBOOK objects
    books[0] = new HardcopyBook("The Lightning Thief", "Rick Riordan", "1111", true, "2005", "A-10");
    books[1] = new HardcopyBook("The Hunger Games", "Suzanne Collins", "2222", true, "2008", "B-07");
    books[2] = new HardcopyBook("The Lion, the Witch and the Wardrobe", "C.S. Lewis", "3333", true, "1950", "C-01");
    books[3] = new HardcopyBook("City of Bones", "Cassandra Clare", "4444", true, "2007", "D-05");
    books[4] = new EBook("A Deadly Education", "Naomi Novik", "5555", true, "2020", "2028-12-31");

    Book::sortBookData(books, SIZE);

    int choice;
    string input;

    while (true) {

        cout << "\n===== LIBRARY MENU =====" << endl;
        cout << "1. Borrow a Book" << endl;
        cout << "2. See Available Books" << endl;
        cout << "3. Return a Book" << endl;
        cout << "4. See Borrowed Books" << endl;
        cout << "5. Exit Program" << endl;
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore();

        // EXIT
        if (choice == 5) {
            cout << "Program terminated." << endl;
            break;
        }

        // AVAILABLE BOOKS
        if (choice == 2) {
            cout << "\n===== AVAILABLE BOOKS =====\n";
            bool any = false;

            for (int i = 0; i < SIZE; i++) {
                if (books[i]->isAvailable()) {
                    books[i]->displayBookDetails();
                    any = true;
                }
            }
            if (!any) cout << "No available books.\n";
            continue;
        }

        // BORROW
        if (choice == 1) {
            cout << "\nEnter ISBN or Title (or 0 to cancel): ";
            getline(cin, input);
            if (input == "0") continue;

            string loweredInput = input;
            transform(loweredInput.begin(), loweredInput.end(), loweredInput.begin(), ::tolower);

            bool found = false;

            for (int i = 0; i < SIZE; i++) {
                string loweredTitle = books[i]->getTitle();
                transform(loweredTitle.begin(), loweredTitle.end(), loweredTitle.begin(), ::tolower);

                bool matchISBN = books[i]->getISBN() == input;
                bool matchTitleExact = loweredTitle == loweredInput;
                bool matchTitlePartial = loweredTitle.find(loweredInput) != string::npos;

                if (matchISBN || matchTitleExact || matchTitlePartial) {
                    found = true;

                    books[i]->displayBookDetails();

                    cout << "Confirm borrow? (y/n): ";
                    char c;
                    cin >> c;
                    cin.ignore();

                    if (c == 'y' || c == 'Y') {
                        if (books[i]->borrowBook())
                            cout << "Book borrowed successfully!\n";
                        else
                            cout << "This book is unavailable.\n";
                    }
                    else {
                        cout << "Borrow cancelled.\n";
                    }

                    break;
                }
            }

            if (!found) cout << "Book not found.\n";
            continue;
        }

        // RETURN BOOK
        if (choice == 3) {
            cout << "\nEnter ISBN to return: ";
            getline(cin, input);

            bool found = false;

            for (int i = 0; i < SIZE; i++) {
                if (books[i]->getISBN() == input) {
                    found = true;

                    cout << "Confirm return? (y/n): ";
                    char c;
                    cin >> c;
                    cin.ignore();

                    if (c == 'y' || c == 'Y') {
                        double fine = 0;
                        books[i]->returnBook(fine);

                        if (fine == 0)
                            cout << "Book returned on time.\n";
                        else
                            cout << "Returned late. Fine: $" << fine << endl;
                    }
                    else {
                        cout << "Return cancelled.\n";
                    }
                    break;
                }
            }

            if (!found) cout << "Book not found.\n";
            continue;
        }

        // BORROWED BOOKS
        if (choice == 4) {
            cout << "\n===== BORROWED BOOKS =====\n";
            bool any = false;

            for (int i = 0; i < SIZE; i++) {
                if (!books[i]->isAvailable()) {
                    books[i]->displayBookDetails();
                    any = true;
                }
            }

            if (!any)
                cout << "No books are currently borrowed.\n";

            continue;
        }

        cout << "Invalid option.\n";
    }

    // CLEAN-UP MEMORY
    for (int i = 0; i < SIZE; i++)
        delete books[i];

    return 0;
}