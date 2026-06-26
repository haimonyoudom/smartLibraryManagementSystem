#include "CategoryUI.h"
#include "helpers.h"
#include "../utils/FileIO.h"
#include <iostream>
using namespace std;

// ── Category operations ───────────────────────

static void addCategory(LinkedList& books, Queue& queue,
                        BST& categories) {
    string cat = readLine("Category name: ");
    if (cat.empty()) { cout << "  ! Name cannot be empty.\n"; return; }
    categories.addCategory(cat);
    saveAll(books, queue, categories);
    cout << "  ✓ Category added.\n";
}

static void deleteCategoryOp(LinkedList& books, HashTable& ht,
                              BST& categories, Stack& history, Queue& queue) {
    cout << "\n-- Delete Category --\n";
    categories.displayCategoriesNumbered();

    string cat = readLine("Category to delete (exact name): ");
    if (cat.empty()) { cout << "  ! Name cannot be empty.\n"; return; }
    if (!categories.searchCategory(cat)) { cout << "  ! Category not found.\n"; return; }

    // Count affected books
    int count = 0;
    Element* e = books.head;
    while (e != nullptr) {
        if (e->data.category == cat) count++;
        e = e->next;
    }

    if (count > 0) {
        cout << "  Warning: " << count << " book(s) in this category will also be deleted.\n";
        if (!confirm("Continue?")) { cout << "  Cancelled.\n"; return; }
    } else {
        if (!confirm("Delete empty category?")) { cout << "  Cancelled.\n"; return; }
    }

    // Collect IDs first — cannot delete while iterating
    int ids[1000];
    int idCount = 0;
    e = books.head;
    while (e != nullptr) {
        if (e->data.category == cat)
            ids[idCount++] = e->data.id;
        e = e->next;
    }

    // Push sentinel so undo knows where this batch starts
    Action sentinel;
    sentinel.type = "CAT_DELETE_START";
    sentinel.meta = cat;
    history.push(sentinel);

    // Push each deleted book to history, then remove
    for (int i = 0; i < idCount; i++) {
        Book* b = ht.searchById(ids[i]);
        if (b != nullptr) {
            Action action;
            action.type         = "DELETE";
            action.bookSnapshot = *b;
            history.push(action);
        }
        syncDelete(books, ht, ids[i]);
    }

    categories.deleteCategory(cat);
    saveAll(books, queue, categories);

    cout << "  ✓ Category \"" << cat << "\" and "
         << idCount << " book(s) deleted.\n";
}

static void mostBorrowed(BST& categories) {
    string cat = categories.getMostBorrowedCategory();
    cout << (cat.empty() ? "  No category data.\n"
                         : "  Most borrowed: " + cat + "\n");
}

// ── Menu ──────────────────────────────────────

void categoryMenu(LinkedList& books, HashTable& ht, Queue& queue,
                  BST& categories, Stack& history) {
    while (true) {
        cout << "\n===== Categories =====\n";
        cout << "  1. Add category\n";
        cout << "  2. Delete category\n";
        cout << "  3. Display all categories\n";
        cout << "  4. Most borrowed category\n";
        cout << "  5. Reload categories from file\n";
        cout << "  0. Back\n";

        int choice = readInt("Choice: ");
        switch (choice) {
            case 0: return;
            case 1: addCategory(books, queue, categories); break;
            case 2: deleteCategoryOp(books, ht, categories, history, queue); break;
            case 3: categories.displayTree(); pause(); break;
            case 4: mostBorrowed(categories); pause(); break;
            case 5:
                loadCategoriesFromFile(categories);
                cout << "  ✓ Categories reloaded.\n";
                break;
            default: cout << "  ! Invalid choice.\n"; break;
        }
    }
}