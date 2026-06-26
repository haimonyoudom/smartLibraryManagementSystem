#include "BookUI.h"
#include "helpers.h"
#include "../sorting/Sort.h"
#include "../utils/FileIO.h"
#include <iostream>
using namespace std;
// ── String helpers ────────────────────────────

static char toLowerChar(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

static string toLowerStr(const string& s) {
    string result = s;
    for (int i = 0; i < (int)result.size(); i++)
        result[i] = toLowerChar(result[i]);
    return result;
}

static bool containsStr(const string& text, const string& query) {
    if (query.empty()) return true;
    if (query.size() > text.size()) return false;
    for (int i = 0; i <= (int)(text.size() - query.size()); i++) {
        bool match = true;
        for (int j = 0; j < (int)query.size(); j++) {
            if (text[i + j] != query[j]) { match = false; break; }
        }
        if (match) return true;
    }
    return false;
}

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

static void updateBook(LinkedList& books, HashTable& ht, Queue& queue,
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

    // ── Auto-assign to waiting students if quantity increased ──
    Book* current = ht.searchById(id);
    while (current->quantity > 0) {
        Student s = queue.dequeueByBookId(id);
        if (s.studentId == 0) break;

        Action autoBorrow;
        autoBorrow.type            = "BORROW";
        autoBorrow.bookSnapshot    = *current;
        autoBorrow.meta            = "AUTO";
        autoBorrow.studentSnapshot = s;
        autoBorrow.hasStudent      = true;
        history.push(autoBorrow);

        current->quantity--;
        current->borrowCount++;
        updateBookStatus(*current);
        books.updateBook(id, *current);
        categories.incrementCount(current->category);
        cout << "  ✓ Automatically given to waiting student: "
             << s.name << " (ID " << s.studentId << ")\n";
    }
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
    string query = readLine("Search title: ");
    string queryLower = toLowerStr(query);

    int matchCount = 0;
    printHeader();

    Element* e = books.head;
    while (e != nullptr) {
        string titleLower = toLowerStr(e->data.title);
        if (containsStr(titleLower, queryLower)) {
            printBookRow(e->data);
            matchCount++;
        }
        e = e->next;
    }

    printDivider();
    if (matchCount == 0)
        cout << "  ! No books found matching \"" << query << "\".\n";
    else
        cout << "  " << matchCount << " result(s) found.\n";

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

    // ── Search queue for ANY student waiting for this book ──
    Student s = queue.dequeueByBookId(id);   // returns empty Student if not found
    if (s.studentId != 0) {                  // assuming 0 means "not found"
        Action autoBorrow;
        autoBorrow.type            = "BORROW";
        autoBorrow.bookSnapshot    = *found;
        autoBorrow.meta            = "AUTO";
        autoBorrow.studentSnapshot = s;
        autoBorrow.hasStudent      = true;
        history.push(autoBorrow);

        found->quantity--;
        found->borrowCount++;
        updateBookStatus(*found);
        books.updateBook(id, *found);
        categories.incrementCount(found->category);
        cout << "  ✓ Automatically given to waiting student: "
             << s.name << " (ID " << s.studentId << ")\n";
    }
}
// ── Undo ──────────────────────────────────────

static void undoAction(LinkedList& books, HashTable& ht, Queue& queue,
                       BST& categories, Stack& history) {
    if (history.isEmpty()) {
        cout << "  ! Nothing to undo.\n";
        return;
    }

    Action last = history.peek();
    cout << "\n-- Undo --\n";

    // ── Category batch delete ──────────────────
    // Stack layout: top→[DELETE]...[DELETE][CAT_DELETE_START]
    // We peek the top DELETE entries to show the user what will be restored
    if (last.type == "DELETE") {
        // Check if there's a CAT_DELETE_START sentinel below
        // by scanning the stack to count consecutive DELETEs then checking sentinel
        StackElement* e = history.top;
        int batchCount = 0;
        while (e != nullptr && e->data.type == "DELETE") {
            batchCount++;
            e = e->next;
        }
        bool isCatBatch = (e != nullptr && e->data.type == "CAT_DELETE_START");

        if (isCatBatch) {
            string catName = e->data.meta;
            cout << "  Last action : DELETE CATEGORY \"" << catName << "\"\n";
            cout << "  Books to restore: " << batchCount << "\n";
            if (!confirm("Undo this action?")) { cout << "  Cancelled.\n"; return; }

            // Pop and restore all books in the batch
            for (int i = 0; i < batchCount; i++) {
                Action a = history.pop();
                Book snap = a.bookSnapshot;
                syncAdd(books, ht, snap);
                categories.addCategory(snap.category);
            }
            // Pop the sentinel
            history.pop();

            // Re-add the category itself (in case it had 0 books)
            categories.addCategory(catName);
            cout << "  ✓ Undone: category \"" << catName
                 << "\" and " << batchCount << " book(s) restored.\n";
            return;
        }
    }

    // ── Single action ──────────────────────────
    if (last.type == "CAT_DELETE_START") {
        // Empty category delete — just pop sentinel and re-add category
        cout << "  Last action : DELETE CATEGORY \"" << last.meta << "\" (empty)\n";
        if (!confirm("Undo this action?")) { cout << "  Cancelled.\n"; return; }
        history.pop();
        categories.addCategory(last.meta);
        cout << "  ✓ Undone: category \"" << last.meta << "\" restored.\n";
        return;
    }

    cout << "  Last action : " << last.type << "\n";
    cout << "  Book        : [" << last.bookSnapshot.id << "] "
         << last.bookSnapshot.title << "\n";
    if (!confirm("Undo this action?")) { cout << "  Cancelled.\n"; return; }

    history.pop();
    Book snap = last.bookSnapshot;

    if (last.type == "ADD") {
        syncDelete(books, ht, snap.id);
        cout << "  ✓ Undone: book [" << snap.id << "] removed.\n";

    } else if (last.type == "DELETE") {
        syncAdd(books, ht, snap);
        categories.addCategory(snap.category);
        cout << "  ✓ Undone: book [" << snap.id << "] restored.\n";

    } else if (last.type == "UPDATE") {
        syncUpdate(books, ht, snap.id, snap);
        cout << "  ✓ Undone: book [" << snap.id << "] restored to previous state.\n";

    } else if (last.type == "BORROW") {
    Book* current = ht.searchById(snap.id);
    if (current == nullptr) { cout << "  ! Book no longer exists.\n"; return; }
    current->quantity++;
    current->borrowCount--;
    updateBookStatus(*current);
    books.updateBook(snap.id, *current);

    // ← re-enqueue the waiting student if this was an auto-borrow
    if (last.hasStudent) {
        queue.enqueueFront(last.studentSnapshot);
        cout << "  ✓ Student " << last.studentSnapshot.name
             << " (ID " << last.studentSnapshot.studentId
             << ") returned to waiting queue.\n";
    }

    cout << "  ✓ Undone: borrow reversed for book [" << snap.id << "].\n";

    } else if (last.type == "RETURN") {
        Book* current = ht.searchById(snap.id);
        if (current == nullptr) { cout << "  ! Book no longer exists.\n"; return; }
        current->quantity--;
        updateBookStatus(*current);
        books.updateBook(snap.id, *current);
        cout << "  ✓ Undone: return reversed for book [" << snap.id << "].\n";

    } else {
        cout << "  ! Unknown action type, cannot undo.\n";
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
        cout << "  9. Undo last action\n";
        cout << "  0. Back\n";

        int choice = readInt("Choice: ");
        switch (choice) {
            case 0: return;
            case 1: addBook(books, ht, categories, history);
                    saveAll(books, queue, categories); break;
            case 2: updateBook(books, ht, queue, categories, history);
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
            case 9: undoAction(books, ht, queue, categories, history);
                    saveAll(books, queue, categories); break;
            default: cout << "  ! Invalid choice.\n"; break;
        }
    }
}