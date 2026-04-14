#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;

// ===============================
// BASE CLASS
// ===============================
class Book {
protected:
    string title;
    string author;
    string isbn;
    bool availability;
    string dateAdd;

public:
    Book(string t, string a, string i, bool avail, string date)
        : title(t), author(a), isbn(i), availability(avail), dateAdd(date) {
    }

    virtual void displayBookDetails() {
        cout << "----------------------------------\n";
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << isbn << endl;
        cout << "Availability: " << (availability ? "Available" : "Borrowed") << endl;
        cout << "Date Added: " << dateAdd << endl;
        cout << "----------------------------------\n";
    }
};

// ===============================
// HARDCOPY BOOK
// ===============================
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
        cout << "----------------------------------\n";
    }
};

// ===============================
// EBOOK
// ===============================
class EBook : public Book {
private:
    string licenseEndDate;

public:
    EBook(string t, string a, string i, bool avail, string date, string license)
        : Book(t, a, i, avail, date), licenseEndDate(license) {
    }

    void displayBookDetails() override {
        Book::displayBookDetails();
        cout << "License End Date: " << licenseEndDate << endl;
        cout << "----------------------------------\n";
    }
};

// ===============================
// A3 TEST APPLICATION
// ===============================
int main() {

    // ✅ CORRECT INITIALISATION
    cout << "===== CORRECT BOOK INITIALISATION =====\n";

    Book* correct1 = new HardcopyBook(
        "The Lightning Thief",
        "Rick Riordan",
        "1111",
        true,
        "2005",
        "A10"
    );

    Book* correct2 = new EBook(
        "A Deadly Education",
        "Naomi Novik",
        "5555",
        true,
        "2020",
        "2030-12-31"
    );

    Book* correct3 = new HardcopyBook(
        "City of Bones",
        "Cassandra Clare",
        "4444",
        true,
        "2007",
        "B07"
    );

    correct1->displayBookDetails();
    correct2->displayBookDetails();
    correct3->displayBookDetails();

    // ❌ INCORRECT INITIALISATION (SAME BOOKS)
    cout << "\n===== INCORRECT BOOK INITIALISATION =====\n";

    Book* incorrect1 = new HardcopyBook(
        "The Lightning Thief",
        "Rick Riordan",
        "ABC123",      // Invalid ISBN
        true,
        "2005",
        "A10"
    );

    Book* incorrect2 = new EBook(
        "A Deadly Education",
        "Naomi Novik",
        "5555",
        true,
        "Year2020",    // Invalid date
        "NoExpiry"     // Invalid license
    );

    Book* incorrect3 = new HardcopyBook(
        "City of Bones",
        "",            // Missing author
        "4444",
        true,
        "2007",
        "B07"
    );

    incorrect1->displayBookDetails();
    incorrect2->displayBookDetails();
    incorrect3->displayBookDetails();

    return 0;
}