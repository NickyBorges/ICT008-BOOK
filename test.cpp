#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Header-classes-library.h"

using namespace std;

// ===============================
// A3 TEST APPLICATION
// ===============================
int main() {

    // ✅ Create Author objects first
    Author author1(1, "Rick Riordan", "American");
    Author author2(2, "Naomi Novik", "American");
    Author author3(3, "Cassandra Clare", "American");

    // ✅ CORRECT INITIALISATION
    cout << "===== CORRECT BOOK INITIALISATION =====\n";

    Book* correct1 = new HardcopyBook(
        "The Lightning Thief",
        author1,
        "1111",
        true,
        "2005",
        "A10"
    );

    Book* correct2 = new EBook(
        "A Deadly Education",
        author2,
        "5555",
        true,
        "2020",
        "2030-12-31"
    );

    Book* correct3 = new HardcopyBook(
        "City of Bones",
        author3,
        "4444",
        true,
        "2007",
        "B07"
    );

    correct1->displayBookDetails();
    correct2->displayBookDetails();
    correct3->displayBookDetails();

    // ❌ INCORRECT INITIALISATION (same books, incorrect data)
    cout << "\n===== INCORRECT BOOK INITIALISATION =====\n";

    Book* incorrect1 = new HardcopyBook(
        "The Lightning Thief",
        author1,
        "ABC123",        // Invalid ISBN
        true,
        "2005",
        "A10"
    );

    Book* incorrect2 = new EBook(
        "A Deadly Education",
        author2,
        "5555",
        true,
        "Year2020",      // Invalid date
        "NoExpiry"       // Invalid licence
    );

    Book* incorrect3 = new HardcopyBook(
        "City of Bones",
        Author(),        // Missing author information
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