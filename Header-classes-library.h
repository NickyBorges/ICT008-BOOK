#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>
using namespace std;

// ==================================================
//                  AUTHOR CLASS
// ==================================================
class Author {
private:
    int authorID;
    string name;
    string nationality;

public:
    Author() : authorID(0), name("Unknown"), nationality("Unknown") {}

    Author(int id, string n, string nat)
        : authorID(id), name(n), nationality(nat) {
    }

    int getAuthorID() const {
        return authorID;
    }

    string getName() const {
        return name;
    }

    string getNationality() const {
        return nationality;
    }

    void displayAuthorDetails() const {
        cout << "Author ID: " << authorID << endl;
        cout << "Author Name: " << name << endl;
        cout << "Nationality: " << nationality << endl;
    }
};

// ==================================================
//                BASE CLASS: BOOK
// ==================================================
class Book {
protected:
    string title;
    Author author;
    string isbn;
    bool availability;
    string dateAdd;

    bool isBorrowed;
    chrono::system_clock::time_point borrowDate;
    chrono::system_clock::time_point dueDate;

public:
    Book(string t, Author a, string i, bool avail, string date)
        : title(t), author(a), isbn(i), availability(avail),
        dateAdd(date), isBorrowed(false) {
    }

    virtual ~Book() {}

    virtual void displayBookDetails() {
        cout << "----------------------------------" << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author.getName() << endl;
        cout << "ISBN: " << isbn << endl;
        cout << "Availability: " << (availability ? "Available" : "Borrowed") << endl;
        cout << "Date Added: " << dateAdd << endl;

        if (!availability) {
            time_t due = chrono::system_clock::to_time_t(dueDate);
            cout << "Due Date: " << ctime(&due);
        }
    }

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
                int lateDays = static_cast<int>(lateHours / 24);
                fineOut = lateDays * 0.30;
            }
            else {
                fineOut = 0.0;
            }
            return true;
        }
        return false;
    }

    string getISBN() const {
        return isbn;
    }

    string getTitle() const {
        return title;
    }

    bool isAvailable() const {
        return availability;
    }

    Author getAuthor() const {
        return author;
    }

    static void sortBookData(Book* books[], int size) {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (books[j]->getISBN() > books[j + 1]->getISBN()) {
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
    HardcopyBook(string t, Author a, string i, bool avail, string date, string shelf)
        : Book(t, a, i, avail, date), shelfNumber(shelf) {
    }

    void displayBookDetails() override {
        Book::displayBookDetails();
        cout << "Type: Hardcopy Book" << endl;
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
    EBook(string t, Author a, string i, bool avail, string date, string licenseEnd)
        : Book(t, a, i, avail, date), licenseEndDate(licenseEnd) {
    }

    void displayBookDetails() override {
        Book::displayBookDetails();
        cout << "Type: EBook" << endl;
        cout << "License End Date: " << licenseEndDate << endl;
        cout << "----------------------------------" << endl;
    }
};

// ==================================================
//                 LIBRARY CLASS
// ==================================================
class Library {
private:
    static const int SIZE = 5;
    Book* books[SIZE];

public:
    Library() {
        for (int i = 0; i < SIZE; i++) {
            books[i] = nullptr;
        }
    }

    ~Library() {
        for (int i = 0; i < SIZE; i++) {
            delete books[i];
        }
    }

    void addBook(int index, Book* book) {
        if (index >= 0 && index < SIZE) {
            books[index] = book;
        }
    }

    void sortBooks() {
        Book::sortBookData(books, SIZE);
    }

    void displayAllBooks() {
        cout << "\n===== ALL BOOKS =====\n";
        for (int i = 0; i < SIZE; i++) {
            if (books[i] != nullptr) {
                books[i]->displayBookDetails();
            }
        }
    }

    void displayAvailableBooks() {
        cout << "\n===== AVAILABLE BOOKS =====\n";
        bool any = false;

        for (int i = 0; i < SIZE; i++) {
            if (books[i] != nullptr && books[i]->isAvailable()) {
                books[i]->displayBookDetails();
                any = true;
            }
        }

        if (!any) {
            cout << "No available books.\n";
        }
    }

    void displayBorrowedBooks() {
        cout << "\n===== BORROWED BOOKS =====\n";
        bool any = false;

        for (int i = 0; i < SIZE; i++) {
            if (books[i] != nullptr && !books[i]->isAvailable()) {
                books[i]->displayBookDetails();
                any = true;
            }
        }

        if (!any) {
            cout << "No books are currently borrowed.\n";
        }
    }

    void borrowBookByISBNOrTitle(const string& input) {
        string loweredInput = input;
        transform(loweredInput.begin(), loweredInput.end(), loweredInput.begin(), ::tolower);

        bool found = false;

        for (int i = 0; i < SIZE; i++) {
            if (books[i] == nullptr) continue;

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

        if (!found) {
            cout << "Book not found.\n";
        }
    }

    void returnBookByISBN(const string& input) {
        bool found = false;

        for (int i = 0; i < SIZE; i++) {
            if (books[i] != nullptr && books[i]->getISBN() == input) {
                found = true;

                cout << "Confirm return? (y/n): ";
                char c;
                cin >> c;
                cin.ignore();

                if (c == 'y' || c == 'Y') {
                    double fine = 0.0;

                    if (books[i]->returnBook(fine)) {
                        if (fine == 0)
                            cout << "Book returned on time.\n";
                        else
                            cout << "Returned late. Fine: $" << fine << endl;
                    }
                    else {
                        cout << "This book was not borrowed.\n";
                    }
                }
                else {
                    cout << "Return cancelled.\n";
                }
                break;
            }
        }

        if (!found) {
            cout << "Book not found.\n";
        }
    }
};
