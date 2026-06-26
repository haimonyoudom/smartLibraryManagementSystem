#include "helpers.h"
#include <iostream>
#include <limits>
#include <sstream>
using namespace std;

// ── Input ─────────────────────────────────────

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& label) {
    int value;
    while (true) {
        cout << label;
        if (cin >> value) { clearInput(); return value; }
        cout << "  ! Invalid number. Try again.\n";
        clearInput();
    }
}

string readLine(const string& label) {
    string value;
    cout << label;
    getline(cin, value);
    return value;
}

string readLineDefault(const string& label, const string& oldValue) {
    cout << label << " [" << oldValue << "]: ";
    string value;
    getline(cin, value);
    return value.empty() ? oldValue : value;
}

int readIntDefault(const string& label, int oldValue) {
    while (true) {
        cout << label << " [" << oldValue << "]: ";
        string value;
        getline(cin, value);
        if (value.empty()) return oldValue;
        istringstream ss(value);
        int number;
        if (ss >> number) return number;
        cout << "  ! Invalid number. Try again.\n";
    }
}

bool confirm(const string& message) {
    while (true) {
        cout << message << " (y/n): ";
        string ans;
        getline(cin, ans);
        if (ans == "y" || ans == "Y") return true;
        if (ans == "n" || ans == "N") return false;
        cout << "  ! Please enter y or n.\n";
    }
}

void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ── Display ───────────────────────────────────

void printDivider() {
    printf("+----+---------------------------+---------------------------+---------------------------+-----+-----------+----------+\n");
}

void printHeader() {
    printDivider();
    printf("| ID | Title                     | Author                    | Category                  | Qty | Status    | Borrowed |\n");
    printDivider();
}

void printBookRow(const Book& b) {
    printf("| %-2d | %-25.25s | %-25.25s | %-25.25s | %-3d | %-9.9s | %-8d |\n",
           b.id,
           b.title.c_str(),
           b.author.c_str(),
           b.category.c_str(),
           b.quantity,
           b.status.c_str(),
           b.borrowCount);
}

void printBook(const Book& b) {
    printHeader();
    printBookRow(b);
    printDivider();
}

// ── Book logic ────────────────────────────────

void updateBookStatus(Book& b) {
    b.status = (b.quantity > 0) ? "available" : "borrowed";
}

void saveAll(LinkedList& books, Queue& queue, BST& categories) {
    saveBooksToCSV(books);
    saveQueueToFile(queue);
    saveCategoriesToFile(categories);
}

void syncAdd(LinkedList& books, HashTable& ht, const Book& b) {
    books.addBook(b);
    ht.insertBook(b);
}

void syncUpdate(LinkedList& books, HashTable& ht, int id, const Book& updated) {
    books.updateBook(id, updated);
    ht.deleteBook(id);
    ht.insertBook(updated);
}

void syncDelete(LinkedList& books, HashTable& ht, int id) {
    books.deleteBook(id);
    ht.deleteBook(id);
}

void buildIndex(LinkedList& books, HashTable& ht) {
    Element* e = books.head;
    while (e != nullptr) {
        ht.insertBook(e->data);
        e = e->next;
    }
}

// ── Category selection ────────────────────────

string selectCategory(BST& categories, const string& current) {
    int total = categories.countCategories();
    if (total == 0) {
        return readLine("Category: ");
    }

    cout << "\nSelect category:\n";
    categories.displayCategoriesNumbered();
    cout << "0. Enter new category";
    if (!current.empty()) cout << " (or keep current [" << current << "])";
    cout << "\n";

    while (true) {
        int choice = readInt("Choice: ");
        if (choice == 0) {
            if (!current.empty()) {
                string cat = readLine("New category (Enter to keep current): ");
                return cat.empty() ? current : cat;
            }
            string cat = readLine("New category: ");
            if (!cat.empty()) return cat;
            cout << "  ! Category cannot be empty.\n";
            continue;
        }
        string cat = categories.getCategoryByIndex(choice);
        if (!cat.empty()) return cat;
        cout << "  ! Invalid choice. Try again.\n";
    }
}