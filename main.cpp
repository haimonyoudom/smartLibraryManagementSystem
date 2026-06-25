#include "utils/FileIO.h"
#include "sorting/Sort.h"
#include "stack/Stack.h"
#include <iostream>
#include <limits>
using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(string label) {
    int value;
    while (true) {
        cout << label;
        if (cin >> value) {
            clearInput();
            return value;
        }

        cout << "Invalid number. Try again.\n";
        clearInput();
    }
}

string readLine(string label) {
    string value;
    cout << label;
    getline(cin, value);
    return value;
}

void updateBookStatus(Book& b) {
    b.status = (b.quantity > 0) ? "available" : "borrowed";
}

void saveAll(LinkedList& books, Queue& queue, BST& categories) {
    saveBooksToCSV(books);
    saveQueueToFile(queue);
    saveCategoriesToFile(categories);
}

void printBook(Book b) {
    printf("+----+---------------------------+---------------------------+---------------------------+-----+-----------+----------+\n");
    printf("| ID | Title                     | Author                    | Category                  | Qty | Status    | Borrowed |\n");
    printf("+----+---------------------------+---------------------------+---------------------------+-----+-----------+----------+\n");
    printf("| %-2d | %-25.25s | %-25.25s | %-25.25s | %-3d | %-9.9s | %-8d |\n",
           b.id,
           b.title.c_str(),
           b.author.c_str(),
           b.category.c_str(),
           b.quantity,
           b.status.c_str(),
           b.borrowCount);
    printf("+----+---------------------------+---------------------------+---------------------------+-----+-----------+----------+\n");
}

Book inputBook(int id = -1) {
    Book b;
    b.id = (id == -1) ? readInt("Book ID: ") : id;
    b.title = readLine("Title: ");
    b.author = readLine("Author: ");
    b.category = readLine("Category: ");
    b.quantity = readInt("Quantity: ");
    updateBookStatus(b);
    b.borrowCount = readInt("Borrow count: ");
    return b;
}

void addBook(LinkedList& books, BST& categories, Stack& history) {
    Book b = inputBook();
    if (books.searchById(b.id) != nullptr) {
        cout << "Book ID already exists.\n";
        return;
    }

    books.addBook(b);
    categories.addCategory(b.category);

    Action action;
    action.type = "ADD";
    action.bookSnapshot = b;
    history.push(action);

    cout << "Book added.\n";
}

void updateBook(LinkedList& books, BST& categories, Stack& history) {
    int id = readInt("Book ID to update: ");
    Element* found = books.searchById(id);
    if (found == nullptr) {
        cout << "Book not found.\n";
        return;
    }

    Action action;
    action.type = "UPDATE";
    action.bookSnapshot = found->data;
    history.push(action);

    Book updated = inputBook(id);
    books.updateBook(id, updated);
    categories.addCategory(updated.category);

    cout << "Book updated.\n";
}

void deleteBook(LinkedList& books, Stack& history) {
    int id = readInt("Book ID to delete: ");
    Element* found = books.searchById(id);
    if (found == nullptr) {
        cout << "Book not found.\n";
        return;
    }

    Action action;
    action.type = "DELETE";
    action.bookSnapshot = found->data;
    history.push(action);

    books.deleteBook(id);
    cout << "Book deleted.\n";
}

void searchBook(LinkedList& books) {
    cout << "1. Search by ID\n";
    cout << "2. Search by title\n";
    int choice = readInt("Choose: ");

    Element* found = nullptr;
    if (choice == 1) {
        int id = readInt("Book ID: ");
        found = books.searchById(id);
    } else if (choice == 2) {
        string title = readLine("Title: ");
        found = books.searchByTitle(title);
    } else {
        cout << "Invalid choice.\n";
        return;
    }

    if (found == nullptr) {
        cout << "Book not found.\n";
    } else {
        printBook(found->data);
    }
}

void displaySortedBooks(LinkedList& books) {
    if (books.isEmpty()) {
        cout << "No books available.\n";
        return;
    }

    cout << "Sort field: id, title, author, category, quantity, borrowCount\n";
    string field = readLine("Field: ");

    Book* arr = Sort::listToArray(books.head, books.getSize());
    Sort::quickSort(arr, 0, books.getSize() - 1, field);

    for (int i = 0; i < books.getSize(); i++) {
        printBook(arr[i]);
    }

    delete[] arr;
}

void borrowBook(LinkedList& books, Queue& queue, BST& categories, Stack& history) {
    int id = readInt("Book ID to borrow: ");
    Element* found = books.searchById(id);
    if (found == nullptr) {
        cout << "Book not found.\n";
        return;
    }

    if (found->data.quantity <= 0) {
        cout << "Book unavailable. Add student to waiting queue.\n";
        Student s;
        s.studentId = readInt("Student ID: ");
        s.name = readLine("Student name: ");
        s.requestedBookId = id;
        queue.enqueue(s);
        return;
    }

    Action action;
    action.type = "BORROW";
    action.bookSnapshot = found->data;
    history.push(action);

    found->data.quantity--;
    found->data.borrowCount++;
    updateBookStatus(found->data);
    categories.incrementCount(found->data.category);

    cout << "Book borrowed.\n";
}

void returnBook(LinkedList& books, Queue& queue, BST& categories, Stack& history) {
    int id = readInt("Book ID to return: ");
    Element* found = books.searchById(id);
    if (found == nullptr) {
        cout << "Book not found.\n";
        return;
    }

    Action action;
    action.type = "RETURN";
    action.bookSnapshot = found->data;
    history.push(action);

    found->data.quantity++;
    updateBookStatus(found->data);
    cout << "Book returned.\n";

    if (!queue.isEmpty() && queue.peek().requestedBookId == id) {
        Student s = queue.dequeue();
        found->data.quantity--;
        found->data.borrowCount++;
        updateBookStatus(found->data);
        categories.incrementCount(found->data.category);
        cout << "Book was given to waiting student: " << s.name << " (ID " << s.studentId << ")\n";
    }
}

void bookMenu(LinkedList& books, Queue& queue, BST& categories, Stack& history) {
    while (true) {
        cout << "\n--- Book Management ---\n";
        cout << "1. Add book\n";
        cout << "2. Update book\n";
        cout << "3. Delete book\n";
        cout << "4. Display all books\n";
        cout << "5. Search book\n";
        cout << "6. Display sorted books\n";
        cout << "7. Borrow book\n";
        cout << "8. Return book\n";
        cout << "0. Back\n";

        int choice = readInt("Choose: ");
        if (choice == 0) return;
        if (choice == 1) {
            addBook(books, categories, history);
            saveAll(books, queue, categories);
        } else if (choice == 2) {
            updateBook(books, categories, history);
            saveAll(books, queue, categories);
        } else if (choice == 3) {
            deleteBook(books, history);
            saveAll(books, queue, categories);
        }
        else if (choice == 4) books.getallBook();
        else if (choice == 5) searchBook(books);
        else if (choice == 6) displaySortedBooks(books);
        else if (choice == 7) {
            borrowBook(books, queue, categories, history);
            saveAll(books, queue, categories);
        } else if (choice == 8) {
            returnBook(books, queue, categories, history);
            saveAll(books, queue, categories);
        }
        else cout << "Invalid choice.\n";
    }
}

void categoryMenu(LinkedList& books, Queue& queue, BST& categories) {
    while (true) {
        cout << "\n--- Categories ---\n";
        cout << "1. Add category\n";
        cout << "2. Delete category\n";
        cout << "3. Search category\n";
        cout << "4. Display categories\n";
        cout << "5. Most borrowed category\n";
        cout << "0. Back\n";

        int choice = readInt("Choose: ");
        if (choice == 0) return;
        if (choice == 1) {
            categories.addCategory(readLine("Category: "));
            saveAll(books, queue, categories);
            cout << "Category added.\n";
        } else if (choice == 2) {
            categories.deleteCategory(readLine("Category: "));
            saveAll(books, queue, categories);
            cout << "Category deleted if it existed.\n";
        } else if (choice == 3) {
            string category = readLine("Category: ");
            cout << (categories.searchCategory(category) ? "Found.\n" : "Not found.\n");
        } else if (choice == 4) {
            categories.displayTree();
        } else if (choice == 5) {
            string category = categories.getMostBorrowedCategory();
            cout << (category.empty() ? "No category data.\n" : "Most borrowed: " + category + "\n");
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

void mainMenu() {
    cout << "\n===== Smart Library System =====\n";
    cout << "1. Book management\n";
    cout << "2. Waiting queue\n";
    cout << "3. Categories\n";
    cout << "4. View history\n";
    cout << "5. Save now\n";
    cout << "0. Exit\n";
}

int main() {
    LinkedList books;
    Queue queue;
    BST categories;
    Stack history;

    loadBooksFromCSV(books,"data/books.csv");
    loadQueueFromFile(queue,"data/queue.txt");
    loadCategoriesFromFile(categories,"data/categories.txt");

    while (true) {
        mainMenu();
        int choice = readInt("Choose: ");

        if (choice == 0) {
            saveAll(books, queue, categories);
            cout << "Goodbye.\n";
            return 0;
        }

        if (choice == 1) bookMenu(books, queue, categories, history);
        else if (choice == 2) queue.display();
        else if (choice == 3) categoryMenu(books, queue, categories);
        else if (choice == 4) history.viewHistory();
        else if (choice == 5) {
            saveAll(books, queue, categories);
            cout << "Saved.\n";
        } else {
            cout << "Invalid choice.\n";
        }
    }
}
