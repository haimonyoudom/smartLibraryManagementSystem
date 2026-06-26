#include "utils/FileIO.h"
#include "stack/Stack.h"
#include "hash_table/hashTable.h"
#include "ui/helpers.h"
#include "ui/BookUI.h"
#include "ui/CategoryUI.h"
#include <iostream>
using namespace std;

static void mainMenu() {
    cout << "\n===== Smart Library System =====\n";
    cout << "  1. Book management\n";
    cout << "  2. Waiting queue\n";
    cout << "  3. Categories\n";
    cout << "  4. View history\n";
    cout << "  0. Exit\n";
}

int main() {
    LinkedList books;
    Queue      queue;
    BST        categories;
    Stack      history;
    HashTable  ht;

    loadBooksFromCSV(books, "data/books.csv");
    loadQueueFromFile(queue, "data/queue.txt");
    loadCategoriesFromFile(categories, "data/categories.txt");
    buildIndex(books, ht);

    while (true) {
        mainMenu();
        int choice = readInt("Choice: ");

        switch (choice) {
            case 0:
                saveAll(books, queue, categories);
                cout << "Goodbye.\n";
                return 0;
            case 1:
                bookMenu(books, ht, queue, categories, history);
                break;
            case 2:
                queue.display();
                pause();
                break;
            case 3:
                categoryMenu(books, ht, queue, categories, history);
                break;
            case 4:
                history.viewHistory();
                pause();
                break;
            default:
                cout << "  ! Invalid choice.\n";
                break;
        }
    }
}