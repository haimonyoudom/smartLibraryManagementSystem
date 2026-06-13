#include <iostream>
#include "../linked_list/LinkedList.h"

using namespace std;

int main()
{
    LinkedList books;

    // Create books
    Book b1 = {1, "Clean Code", "Robert Martin", "Programming", 5, "available", 0};
    Book b2 = {2, "Design Patterns", "GoF", "Programming", 3, "available", 0};
    Book b3 = {3, "C++ Primer", "Lippman", "Programming", 4, "available", 0};

    // Test addBook
    cout << "=== Add Books ===" << endl;
    books.addBook(b1);
    books.addBook(b2);
    books.addBook(b3);

    books.getallBook();

    // Test getSize
    cout << "\nSize: " << books.getSize() << endl;

    // Test searchById
    cout << "\n=== Search By ID ===" << endl;

    Element* found = books.searchById(2);

    if (found != nullptr)
    {
        cout << "Found Book: "
             << found->data.title
             << " by "
             << found->data.author
             << endl;
    }
    else
    {
        cout << "Book not found" << endl;
    }

    // Test updateBook
    cout << "\n=== Update Book ID 2 ===" << endl;

    Book updatedBook = {
        2,
        "Design Patterns Updated",
        "Gang of Four",
        "Software Engineering",
        10,
        "available",
        5
    };

    if (books.updateBook(2, updatedBook))
    {
        cout << "Update successful!" << endl;
    }
    else
    {
        cout << "Update failed!" << endl;
    }

    books.getallBook();

    // Test deleteBook
    cout << "\n=== Delete Book ID 1 ===" << endl;

    if (books.deleteBook(1))
    {
        cout << "Delete successful!" << endl;
    }
    else
    {
        cout << "Delete failed!" << endl;
    }

    books.getallBook();

    // Test delete non-existing book
    cout << "\n=== Delete Book ID 99 ===" << endl;

    if (books.deleteBook(99))
    {
        cout << "Delete successful!" << endl;
    }
    else
    {
        cout << "Book not found!" << endl;
    }

    // Test isEmpty
    cout << "\n=== isEmpty ===" << endl;

    if (books.isEmpty())
    {
        cout << "List is empty" << endl;
    }
    else
    {
        cout << "List is NOT empty" << endl;
    }

    cout << "\nFinal Size: " << books.getSize() << endl;

    return 0;
}