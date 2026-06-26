#include "BookUI.h"
#include "helpers.h"
#include "../sorting/Sort.h"
#include "../utils/FileIO.h"
#include <iostream>
using namespace std;

// ── Internal helpers ──────────────────────────

static string chooseSortField() {
    const string fields[] = {"id", "title", "author", "category", "quantity", "borrowCount"};
    const string labels[] = {"ID", "Title", "Author", "Category", "Quantity", "Borrow count"};
    cout << "\nSort by:\n";
    for (int i = 0; i < 6; i++)
        cout << "  " << (i + 1) << ". " << labels[i] << "\n";
    while (true) {
        int c = readInt("Choice: ");
        if (c >= 1 && c <= 6) return fields[c - 1];
        cout << "  ! Invalid choice.\n";
    }
}

static Book inputBook(BST& categories, HashTable& ht) {
    Book b;
    while (true) {
        b.id = readInt("Book ID: ");
        if (ht.searchById(b.id) == nullptr) break;
        cout << "  ! ID " << b.id << " already exists. Choose a different ID.\n";
    }
    b.title       = readLine("Title: ");
    b.author      = readLine("Author: ");
    b.category    = selectCategory(categories);
    b.quantity    = readInt("Quantity: ");
    b.borrowCount = 0;
    updateBookStatus(b);
    return b;
}

static Book inputBookUpdate(const Book& old, BST& categories) {
    Book b;
    b.id          = old.id;
    b.title       = readLineDefault("Title", old.title);
    b.author      = readLineDefault("Author", old.author);
    b.category    = selectCategory(categories, old.category);
    b.quantity    = readIntDefault("Quantity", old.quantity);
    b.borrowCount = old.borrowCount;
    updateBookStatus(b);
    return b;
}

// ── Book operations ───────────────────────────

static void addBook(LinkedList& books, HashTable& ht,
                    BST& categories, Stack& history) {
    cout << "\n-- Add Book --\n";
    Book b = inputBook(categories, ht);

    syncAdd(books, ht, b);
    categories.addCategory(b.category);

    Action action;
    action.type         = "ADD";
    action.bookSnapshot = b;
    history.push(action);

    saveBooksToCSV(books);
    saveCategoriesToFile(categories);
    cout << "  ✓ Book added.\n";
}

static void updateBook(LinkedList& books, HashTable& ht,
                       BST& categories, Stack& history) {
    cout << "\n-- Update Book --\n";
    int id = readInt("Book ID to update: ");
    Book* found = ht.searchById(id);
    if (found == nullptr) { cout << "  ! Book not found.\n"; return; }

    printBook(*found);

    Action action;
    action.type         = "UPDATE";
    action.bookSnapshot = *found;
    history.push(action);

    Book updated = inputBookUpdate(*found, categories);
    syncUpdate(books, ht, id, updated);
    categories.addCategory(updated.category);

    cout << "  ✓ Book updated.\n";
}

static void deleteBook(LinkedList& books, HashTable& ht, Stack& history) {
    cout << "\n-- Delete Book --\n";
    int id = readInt("Book ID to delete: ");
    Book* found = ht.searchById(id);
    if (found == nullptr) { cout << "  ! Book not found.\n"; return; }

    printBook(*found);
    if (!confirm("Delete this book?")) { cout << "  Cancelled.\n"; return; }

    Action action;
    action.type         = "DELETE";
    action.bookSnapshot = *found;
    history.push(action);

    syncDelete(books, ht, id);
    cout << "  ✓ Book deleted.\n";
}

static void searchBook(HashTable& ht, LinkedList& books) {
    cout << "\n-- Search Book --\n";
    cout << "  1. Search by ID\n";
    cout << "  2. Search by title\n";
    int choice = readInt("Choice: ");

    if (choice == 1) {
        int id = readInt("Book ID: ");
        Book* found = ht.searchById(id);
        if (found == nullptr) cout << "  ! Book not found.\n";
        else printBook(*found);

    } else if (choice == 2) {
        string title = readLine("Title: ");
        Element* found = books.searchByTitle(title);
        if (found == nullptr) cout << "  ! Book not found.\n";
        else printBook(found->data);

    } else {
        cout << "  ! Invalid choice.\n";
    }
}

static void displayAllBooks(LinkedList& books) {
    cout << "\n-- All Books --\n";
    if (books.isEmpty()) { cout << "  No books in library.\n"; return; }
    books.getallBook();
}

static void displaySortedBooks(LinkedList& books) {
    cout << "\n-- Sorted Books --\n";
    if (books.isEmpty()) { cout << "  No books in library.\n"; return; }

    string field = chooseSortField();
    int n = books.getSize();
    Book* arr = Sort::listToArray(books.head, n);
    Sort::quickSort(arr, 0, n - 1, field);

    printHeader();
    for (int i = 0; i < n; i++) printBookRow(arr[i]);
    printDivider();

    delete[] arr;
}

static void borrowBook(LinkedList& books, HashTable& ht, Queue& queue,
                       BST& categories, Stack& history) {
    cout << "\n-- Borrow Book --\n";
    int id = readInt("Book ID to borrow: ");
    Book* found = ht.searchById(id);
    if (found == nullptr) { cout << "  ! Book not found.\n"; return; }

    printBook(*found);

    if (found->quantity <= 0) {
        cout << "  Book unavailable. Add student to waiting queue?\n";
        if (!confirm("Add to queue?")) return;
        Student s;
        s.studentId       = readInt("Student ID: ");
        s.name            = readLine("Student name: ");
        s.requestedBookId = id;
        queue.enqueue(s);
        cout << "  ✓ Student added to waiting queue.\n";
        return;
    }

    Action action;
    action.type         = "BORROW";
    action.bookSnapshot = *found;
    history.push(action);

    found->quantity--;
    found->borrowCount++;
    updateBookStatus(*found);
    books.updateBook(id, *found);
    categories.incrementCount(found->category);

    cout << "  ✓ Book borrowed.\n";
}

static void returnBook(LinkedList& books, HashTable& ht, Queue& queue,
                       BST& categories, Stack& history) {
    cout << "\n-- Return Book --\n";
    int id = readInt("Book ID to return: ");
    Book* found = ht.searchById(id);
    if (found == nullptr) { cout << "  ! Book not found.\n"; return; }

    Action action;
    action.type         = "RETURN";
    action.bookSnapshot = *found;
    history.push(action);

    found->quantity++;
    updateBookStatus(*found);
    books.updateBook(id, *found);
    cout << "  ✓ Book returned.\n";

    if (!queue.isEmpty() && queue.peek().requestedBookId == id) {
        Student s = queue.dequeue();
        found->quantity--;
        found->borrowCount++;
        updateBookStatus(*found);
        books.updateBook(id, *found);
        categories.incrementCount(found->category);
        cout << "  ✓ Automatically given to waiting student: "
             << s.name << " (ID " << s.studentId << ")\n";
    }
}

// ── Menu ──────────────────────────────────────

void bookMenu(LinkedList& books, HashTable& ht, Queue& queue,
              BST& categories, Stack& history) {
    while (true) {
        cout << "\n===== Book Management =====\n";
        cout << "  1. Add book\n";
        cout << "  2. Update book\n";
        cout << "  3. Delete book\n";
        cout << "  4. Display all books\n";
        cout << "  5. Search book\n";
        cout << "  6. Display sorted books\n";
        cout << "  7. Borrow book\n";
        cout << "  8. Return book\n";
        cout << "  0. Back\n";

        int choice = readInt("Choice: ");
        switch (choice) {
            case 0: return;
            case 1: addBook(books, ht, categories, history);
                    saveAll(books, queue, categories); break;
            case 2: updateBook(books, ht, categories, history);
                    saveAll(books, queue, categories); break;
            case 3: deleteBook(books, ht, history);
                    saveAll(books, queue, categories); break;
            case 4: displayAllBooks(books); pause(); break;
            case 5: searchBook(ht, books); pause(); break;
            case 6: displaySortedBooks(books); pause(); break;
            case 7: borrowBook(books, ht, queue, categories, history);
                    saveAll(books, queue, categories); break;
            case 8: returnBook(books, ht, queue, categories, history);
                    saveAll(books, queue, categories); break;
            default: cout << "  ! Invalid choice.\n"; break;
        }
    }
}