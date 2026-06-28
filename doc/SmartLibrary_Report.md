# Smart Library Management System

## Project Report - Data Structures and Algorithms in C++

**Project Type:** Console-based library management system  
**Language:** C++  
**Main Topic:** Practical use of data structures and algorithms  

---

## Table of Contents

1. [Introduction](#1-introduction)
2. [Problem Statement](#2-problem-statement)
3. [System Design](#3-system-design)
4. [Data Structures Used](#4-data-structures-used)
5. [Algorithms Used](#5-algorithms-used)
6. [Complexity Analysis](#6-complexity-analysis)
7. [Testing Results](#7-testing-results)
8. [Conclusion](#8-conclusion)

---

<div style="page-break-after: always;"></div>

## 1. Introduction

The **Smart Library Management System** is a console-based C++ application designed for managing a small library. The system allows a librarian or user to manage books, categories, borrowing activity, waiting requests, sorting, searching, undo history, and file-based persistence.

This project was developed as a Data Structures and Algorithms course project. Its main purpose is not only to provide library management features, but also to demonstrate how different data structures can work together inside one complete program.

The system uses six major data structure concepts:

- Linked List
- Hash Table
- Queue
- Stack
- Binary Search Tree
- Sorting arrays

Each structure has a specific responsibility. For example, the Linked List stores all books, the Hash Table supports fast book lookup by ID, the Queue manages waiting students, the Stack stores undo history, the Binary Search Tree stores categories, and sorting algorithms are used to display books in ordered form.

The application is organized into separate modules for data types, data structures, file input/output, user interface logic, and business operations. This makes the project easier to understand, test, and maintain.

---

## 2. Problem Statement

Libraries need to manage books, availability, borrowing history, categories, and waiting requests. If these tasks are handled manually, the process can become slow and error-prone, especially when many books and students are involved.

This project solves the problem by building an automated library system that can store book information, search quickly, handle borrowing and returning, manage waiting students, organize books by category, sort book records, save data to files, and undo recent operations.

### 2.1 Main Problems

| Problem | Explanation |
|---|---|
| Book inventory management | The system must store book ID, title, author, category, quantity, status, and borrow count. |
| Fast searching | Searching by book ID should be fast because it is used in update, delete, borrow, return, and lookup operations. |
| Flexible title search | Users should be able to search by partial title without needing the exact full title. |
| Borrow waiting list | If a book has no available quantity, students should be placed in a fair waiting queue. |
| Category management | Books should be grouped by category, and category borrow counts should be tracked. |
| Undo support | Users should be able to reverse recent actions such as adding, deleting, updating, borrowing, returning, and category deletion. |
| Data persistence | Books, categories, and waiting queue data should be saved and loaded from files. |

### 2.2 Project Requirements

The system is designed to support the following requirements:

- Add, update, delete, display, and search books.
- Search books by ID using a Hash Table with average-case `O(1)` lookup.
- Search books by partial title using case-insensitive matching.
- Display all books or display books sorted by selected fields.
- Borrow a book when quantity is available.
- Add a student to the waiting queue when a requested book is unavailable.
- Return a book and automatically assign it to a waiting student if possible.
- Update book quantity and automatically assign available copies to waiting students.
- Store categories in a Binary Search Tree.
- Track borrow counts per category.
- Delete a category and cascade-delete all books in that category.
- Use a Stack to undo recent actions.
- Save and load data using files.

---

<div style="page-break-after: always;"></div>

## 3. System Design

### 3.1 Architecture Overview

The project uses a modular design. Each part of the system has a clear responsibility.

| Layer | Components | Responsibility |
|---|---|---|
| Data Model Layer | `utils/type.h` | Defines the main structs: `Book`, `Student`, and `Action`. |
| Data Structure Layer | `linked_list`, `hash_table`, `queue`, `stack`, `tree`, `sorting` | Implements the main data structures and algorithms. |
| Persistence Layer | `utils/FileIO.h`, `utils/FileIO.cpp` | Loads and saves books, queue data, and categories. |
| User Interface Layer | `ui/helpers`, `ui/BookUI`, `ui/CategoryUI` | Handles menus, user input, display tables, and operation flow. |
| Application Entry | `main.cpp` | Initializes all structures, loads data, builds the index, and starts the main menu. |

### 3.2 Project File Structure

| Folder | File | Purpose |
|---|---|---|
| Root | `main.cpp` | Program entry point and main menu. |
| `utils/` | `type.h` | Defines `Book`, `Student`, and `Action`. |
| `utils/` | `FileIO.h`, `FileIO.cpp` | Handles file loading and saving. |
| `linked_list/` | `LinkedList.h`, `LinkedList.cpp` | Stores the main book collection. |
| `hash_table/` | `hashTable.h`, `hashTable.cpp` | Provides fast lookup by book ID. |
| `queue/` | `Queue.h`, `Queue.cpp` | Stores students waiting for unavailable books. |
| `stack/` | `Stack.h`, `Stack.cpp` | Stores action history for undo operations. |
| `tree/` | `tree.h`, `tree.cpp` | Manages categories using a Binary Search Tree. |
| `sorting/` | `Sort.h`, `Sort.cpp` | Contains Bubble Sort, Insertion Sort, Merge Sort, and QuickSort. |
| `ui/` | `helpers.h`, `helpers.cpp` | Shared input, display, save, and sync helper functions. |
| `ui/` | `BookUI.h`, `BookUI.cpp` | Book menu operations. |
| `ui/` | `CategoryUI.h`, `CategoryUI.cpp` | Category menu operations. |
| `data/` | `books.csv` | Stores persistent book records. |
| `data/` | `queue.txt` | Stores persistent waiting queue records. |
| `data/` | `categories.txt` | Stores persistent category records. |

### 3.3 Main Program Flow

When the program starts, `main.cpp` creates the main objects:

- `LinkedList books`
- `Queue queue`
- `BST categories`
- `Stack history`
- `HashTable ht`

Then the program loads saved data:

```cpp
loadBooksFromCSV(books, "data/books.csv");
loadQueueFromFile(queue, "data/queue.txt");
loadCategoriesFromFile(categories, "data/categories.txt");
buildIndex(books, ht);
```

Books are first loaded into the Linked List. After that, `buildIndex()` inserts each book into the Hash Table so that ID-based searching becomes faster during runtime.

### 3.4 Data Synchronization

The project uses both a Linked List and a Hash Table for books:

- The **Linked List** is the main storage for all books.
- The **Hash Table** is used for fast ID search.

Because the same book data exists in both structures, the program uses helper functions to keep them synchronized:

| Helper Function | Purpose |
|---|---|
| `syncAdd()` | Adds a book to both the Linked List and Hash Table. |
| `syncUpdate()` | Updates a book in the Linked List and refreshes it in the Hash Table. |
| `syncDelete()` | Deletes a book from both the Linked List and Hash Table. |
| `buildIndex()` | Builds the Hash Table from the Linked List on startup. |

### 3.5 File Persistence

The system saves data to files so that information is not lost after the program exits.

| Data | File | Format |
|---|---|---|
| Books | `data/books.csv` | `id,title,author,category,quantity,status,borrowCount` |
| Waiting queue | `data/queue.txt` | `studentId,name,requestedBookId` |
| Categories | `data/categories.txt` | `category,borrowCount` |

After write operations such as add, update, delete, borrow, return, undo, or category changes, the system saves data using `saveAll()`.

---

<div style="page-break-after: always;"></div>

## 4. Data Structures Used

### 4.1 Overview

| Data Structure | Main Use in System | Important Operations |
|---|---|---|
| Linked List | Main book storage | Add, update, delete, traverse, display |
| Hash Table | Fast book search by ID | Insert, search, delete |
| Queue | Waiting list for unavailable books | Enqueue, dequeue, targeted dequeue |
| Stack | Undo history | Push, pop, peek, view history |
| Binary Search Tree | Category management | Insert, delete, search, inorder traversal |
| Array | Temporary structure for sorting | Convert list to array, sort, display |

### 4.2 Linked List

The Linked List is used as the main storage for book records. Each node stores one `Book` object and a pointer to the next node.

Each book contains:

- Book ID
- Title
- Author
- Category
- Quantity
- Status
- Borrow count

The Linked List is suitable for this project because books can be added dynamically without needing a fixed array size. It also supports traversal, which is useful for displaying all books, searching by title, saving books to file, and converting the list into an array for sorting.

Important operations:

| Operation | Description |
|---|---|
| `addBook()` | Adds a new book at the end of the list. |
| `updateBook()` | Finds a book by ID and replaces its stored data. |
| `deleteBook()` | Removes a book node by ID. |
| `searchById()` | Searches the list linearly by book ID. |
| `getallBook()` | Displays all books in table format. |

The project also uses the Linked List for partial title search by manually traversing all book nodes.

### 4.3 Hash Table

The Hash Table is used to make book searching by ID faster. Instead of scanning the whole Linked List every time, the program computes a bucket index using the book ID.

The hash function is:

```cpp
int hashFunction(int key) {
    return key % HT_SIZE;
}
```

In this project, `HT_SIZE` is 10. If two book IDs produce the same bucket index, the Hash Table handles the collision using separate chaining. Each bucket stores a linked list of hash nodes.

Important operations:

| Operation | Description |
|---|---|
| `insertBook()` | Inserts a book into the correct hash bucket. |
| `searchById()` | Finds a book by ID using the hash bucket. |
| `deleteBook()` | Removes a book from its hash bucket. |
| `display()` | Displays the hash table buckets. |

The Hash Table is used in book search, update, delete, borrow, and return operations.

### 4.4 Queue

The Queue stores students waiting for unavailable books. It follows the FIFO principle: **First In, First Out**.

When a student tries to borrow a book with quantity `0`, the system can add the student to the waiting queue. When a copy becomes available, the system searches the queue for the first student waiting for that specific book.

Important operations:

| Operation | Description |
|---|---|
| `enqueue()` | Adds a student to the rear of the queue. |
| `dequeue()` | Removes the front student. |
| `peek()` | Shows the front student without removing them. |
| `dequeueByBookId()` | Removes the first student waiting for a specific book ID. |
| `enqueueFront()` | Adds a student to the front of the queue, used when undoing an auto-borrow. |
| `display()` | Displays the waiting queue. |

The `dequeueByBookId()` function is important because the queue may contain students waiting for different books. The function scans the queue and removes only the first student whose `requestedBookId` matches the returned or updated book.

### 4.5 Stack

The Stack stores action history for undo operations. It follows the LIFO principle: **Last In, First Out**.

Each stack node stores an `Action` object. The `Action` struct contains:

- `type`
- `bookSnapshot`
- `meta`
- `studentSnapshot`
- `hasStudent`

The stack supports undo for these actions:

| Action Type | Meaning |
|---|---|
| `ADD` | A book was added. |
| `UPDATE` | A book was updated. |
| `DELETE` | A book was deleted. |
| `BORROW` | A book was borrowed. |
| `RETURN` | A book was returned. |
| `CAT_DELETE_START` | A category deletion batch started. |

The undo system stores snapshots before changing data. For example, before updating a book, the old book data is pushed onto the Stack. If the user chooses undo, the system restores the previous book snapshot.

The Stack also supports category deletion undo. When a category is deleted, the system pushes a `CAT_DELETE_START` sentinel action first, then pushes `DELETE` actions for each affected book. This allows the program to restore all deleted books and the category as one batch.

### 4.6 Binary Search Tree

The Binary Search Tree stores book categories alphabetically. Each tree node stores:

- Category name
- Borrow count
- Left child pointer
- Right child pointer

The BST is useful because inorder traversal displays categories in sorted alphabetical order.

Important operations:

| Operation | Description |
|---|---|
| `addCategory()` | Inserts a category into the tree. |
| `deleteCategory()` | Removes a category from the tree. |
| `searchCategory()` | Searches for a category. |
| `incrementCount()` | Increases the borrow count of a category. |
| `setCount()` | Sets a category borrow count when loading from file. |
| `getMostBorrowedCategory()` | Finds the category with the highest borrow count. |
| `displayTree()` | Displays categories using inorder traversal. |
| `saveCategories()` | Saves categories to file using inorder traversal. |

### 4.7 Sorting Array

The book records are stored in a Linked List, but sorting a Linked List directly is less convenient. Therefore, the system converts the Linked List into a dynamic array using `Sort::listToArray()`.

After conversion, the array can be sorted by:

- ID
- Title
- Author
- Category
- Quantity
- Borrow count

The original Linked List is not modified when displaying sorted books. The sorted array is only used temporarily for display.

---

<div style="page-break-after: always;"></div>

## 5. Algorithms Used

### 5.1 Hashing Algorithm

The Hash Table uses a modulo-based hash function:

```cpp
int hashFunction(int key) {
    return key % HT_SIZE;
}
```

The book ID is divided by the table size, and the remainder becomes the bucket index.

Example:

```text
Book ID = 123
HT_SIZE = 10
123 % 10 = 3
```

Therefore, book ID `123` is stored in bucket `3`.

If multiple IDs map to the same bucket, the system uses separate chaining. This means each bucket can store multiple nodes in a linked chain.

### 5.2 Case-Insensitive Partial Title Search

The title search algorithm allows the user to search using only part of a title. It is also case-insensitive.

The process is:

1. Read the search text from the user.
2. Convert the search text to lowercase.
3. Traverse all books in the Linked List.
4. Convert each book title to lowercase.
5. Check whether the search text appears inside the title.
6. Display all matching books.

The helper functions used are:

```cpp
static string toLowerStr(const string& s);
static bool containsStr(const string& text, const string& query);
```

This makes searches more flexible. For example, searching for `data` can match titles such as `Data Structures`, `Database System`, or `Introduction to Data Science`.

### 5.3 QuickSort

QuickSort is the sorting algorithm used by the book display menu. The program first converts the Linked List into an array, then calls:

```cpp
Sort::quickSort(arr, 0, n - 1, field);
```

The implemented QuickSort uses the **first element** as the pivot:

```cpp
Book pivot = arr[lb];
```

The partition function moves smaller or equal values to one side and larger values to the other side according to the selected field. After partitioning, QuickSort recursively sorts the left and right parts of the array.

QuickSort is efficient on average, with average time complexity `O(n log n)`. However, because the implementation uses the first element as pivot, it can degrade to `O(n^2)` when the input order is already unfavorable, such as already sorted or reverse-sorted data.

### 5.4 MergeSort

MergeSort is also implemented in the sorting module. It uses a divide-and-conquer approach:

1. Divide the array into two halves.
2. Recursively sort both halves.
3. Merge the sorted halves back together.

The merge step creates one temporary array:

```cpp
Book* b = new Book[n];
```

MergeSort has guaranteed `O(n log n)` time complexity, but it requires `O(n)` extra memory.

### 5.5 Bubble Sort

Bubble Sort repeatedly compares adjacent elements and swaps them if they are in the wrong order.

The implementation includes an optimization using a flag. If no swaps occur during a full pass, the algorithm stops early because the array is already sorted.

Bubble Sort is simple and useful for demonstration, but it is inefficient for large datasets because its average and worst-case time complexity is `O(n^2)`.

### 5.6 Insertion Sort

Insertion Sort builds the sorted part of the array one element at a time. It takes each element and inserts it into the correct position among the already sorted elements.

Insertion Sort is efficient for small arrays or nearly sorted data. Its best-case time complexity is `O(n)`, but its average and worst-case time complexity is `O(n^2)`.

### 5.7 BST Inorder Traversal

The Binary Search Tree uses inorder traversal:

```text
Left subtree -> Current node -> Right subtree
```

Because categories are inserted using string comparison, inorder traversal displays categories alphabetically.

Inorder traversal is used for:

- Displaying categories.
- Displaying numbered categories.
- Saving categories to file.
- Finding the most borrowed category.

### 5.8 Undo Algorithm

The undo system uses the Stack. When a state-changing operation happens, the program pushes an action snapshot before or during the operation. The most recent action is always at the top of the Stack.

Undo behavior:

| Action | Undo Operation |
|---|---|
| `ADD` | Delete the added book. |
| `DELETE` | Restore the deleted book. |
| `UPDATE` | Restore the previous book data. |
| `BORROW` | Increase quantity and decrease borrow count. |
| `RETURN` | Decrease quantity. |
| Auto-borrow `BORROW` | Reverse borrow and return the student to the front of the queue. |
| Category deletion batch | Restore all deleted books and re-add the category. |

Auto-borrow undo is handled using:

- `hasStudent = true`
- `studentSnapshot`
- `enqueueFront()`

This allows the waiting student to be returned to the front of the queue when the automatic borrow is undone.

### 5.9 Auto-Assign Algorithm

The auto-assign feature runs when:

- A book is returned.
- A book's quantity is increased during update.

The system calls:

```cpp
queue.dequeueByBookId(id);
```

If a matching student is found, the system immediately gives the available book to that student. The book quantity decreases, the borrow count increases, and an auto-borrow action is pushed to the undo Stack.

For quantity updates, the system loops while copies are available, allowing multiple waiting students to receive books if the quantity was increased enough.

---

<div style="page-break-after: always;"></div>

## 6. Complexity Analysis

### 6.1 Time and Space Complexity Table

In this table, `n` is the number of books, `k` is the number of categories, `q` is the number of waiting students, and `m` is the length-based matching cost for title search.

| Operation | Best Time | Average Time | Worst Time | Auxiliary Space |
|---|---:|---:|---:|---:|
| Linked List `addBook()` | `O(1)` | `O(1)` | `O(1)` | `O(1)` |
| Linked List `deleteBook()` | `O(1)` | `O(n)` | `O(n)` | `O(1)` |
| Linked List `updateBook()` | `O(1)` | `O(n)` | `O(n)` | `O(1)` |
| Linked List `searchById()` | `O(1)` | `O(n)` | `O(n)` | `O(1)` |
| Partial title search | `O(1)` | `O(n * m)` | `O(n * m)` | `O(1)` |
| Hash Table `insertBook()` | `O(1)` | `O(1)` | `O(n)` | `O(1)` |
| Hash Table `searchById()` | `O(1)` | `O(1)` | `O(n)` | `O(1)` |
| Hash Table `deleteBook()` | `O(1)` | `O(1)` | `O(n)` | `O(1)` |
| Queue `enqueue()` | `O(1)` | `O(1)` | `O(1)` | `O(1)` |
| Queue `dequeue()` | `O(1)` | `O(1)` | `O(1)` | `O(1)` |
| Queue `enqueueFront()` | `O(1)` | `O(1)` | `O(1)` | `O(1)` |
| Queue `dequeueByBookId()` | `O(1)` | `O(q)` | `O(q)` | `O(1)` |
| Stack `push()` | `O(1)` | `O(1)` | `O(1)` | `O(1)` |
| Stack `pop()` | `O(1)` | `O(1)` | `O(1)` | `O(1)` |
| BST `addCategory()` | `O(1)` | `O(log k)` | `O(k)` | `O(h)` recursion |
| BST `searchCategory()` | `O(1)` | `O(log k)` | `O(k)` | `O(h)` recursion |
| BST `deleteCategory()` | `O(1)` | `O(log k)` | `O(k)` | `O(h)` recursion |
| BST `getMostBorrowedCategory()` | `O(k)` | `O(k)` | `O(k)` | `O(h)` recursion |
| Convert Linked List to array | `O(n)` | `O(n)` | `O(n)` | `O(n)` |
| QuickSort | `O(n log n)` | `O(n log n)` | `O(n^2)` | `O(log n)` average recursion |
| MergeSort | `O(n log n)` | `O(n log n)` | `O(n log n)` | `O(n)` |
| Bubble Sort | `O(n)` | `O(n^2)` | `O(n^2)` | `O(1)` |
| Insertion Sort | `O(n)` | `O(n^2)` | `O(n^2)` | `O(1)` |
| Save books to file | `O(n)` | `O(n)` | `O(n)` | `O(1)` |
| Save queue to file | `O(q)` | `O(q)` | `O(q)` | `O(q)` temporary array |
| Save categories to file | `O(k)` | `O(k)` | `O(k)` | `O(h)` recursion |

### 6.2 Key Complexity Observations

The Hash Table is the most important performance improvement in the project. Without it, ID-based operations would require scanning the Linked List in `O(n)` time. With the Hash Table, searching by ID is `O(1)` on average.

The Linked List remains useful because it supports full traversal for display, file saving, partial title search, and conversion to an array for sorting.

The Queue provides `O(1)` insertion at the rear for normal waiting-list behavior. The special `dequeueByBookId()` operation is `O(q)` because it may need to scan the queue to find the first student waiting for a specific book.

The Stack provides `O(1)` push and pop, making undo operations efficient. Some undo actions still call other operations, such as deleting a book or restoring a book, so the total undo cost depends on the action being reversed.

The BST gives average-case `O(log k)` category insert, search, and delete operations. In the worst case, if categories are inserted in sorted order, the tree can become unbalanced and operations may become `O(k)`.

File loading and saving are linear in the number of records. This is acceptable for this project because the data files are small and file operations happen during startup, exit, and after write operations.

---

<div style="page-break-after: always;"></div>

## 7. Testing Results

Testing was performed manually through the console menus and by checking the saved data files. The purpose of testing was to verify that the main features work correctly and that the data structures stay synchronized.

### 7.1 Functional Test Cases

| No. | Test Case | Expected Result | Actual Result | Status |
|---:|---|---|---|---|
| 1 | Add a book with a new ID | Book is stored successfully. | Book was added to Linked List and Hash Table. | Pass |
| 2 | Add a book with an existing ID | System rejects duplicate ID. | Duplicate ID was rejected. | Pass |
| 3 | Search book by existing ID | Matching book is displayed. | Correct book was found using Hash Table. | Pass |
| 4 | Search book by missing ID | System shows not found message. | Not found message displayed. | Pass |
| 5 | Search by partial lowercase title | Matching books are displayed. | Matching titles were displayed. | Pass |
| 6 | Search by partial mixed-case title | Search ignores case. | Correct case-insensitive results were displayed. | Pass |
| 7 | Update book information | Book details are changed. | Linked List and Hash Table were updated. | Pass |
| 8 | Delete a book | Book is removed from system. | Book was removed from Linked List and Hash Table. | Pass |
| 9 | Borrow available book | Quantity decreases and borrow count increases. | Quantity and borrow count updated correctly. | Pass |
| 10 | Borrow unavailable book | Student can be added to waiting queue. | Student was enqueued. | Pass |
| 11 | Return book with no waiting student | Quantity increases. | Quantity increased correctly. | Pass |
| 12 | Return book with waiting student | Book is auto-assigned to first matching student. | Matching student was dequeued and book was borrowed automatically. | Pass |
| 13 | Increase quantity while students are waiting | Available copies are assigned to waiting students. | Waiting students were served while quantity was available. | Pass |
| 14 | Display waiting queue | Queue appears in FIFO order. | Queue displayed correctly. | Pass |
| 15 | Add category | Category is inserted into BST. | Category appeared in category display. | Pass |
| 16 | Delete empty category | Category is removed and undoable. | Category was deleted and undo restored it. | Pass |
| 17 | Delete category with books | Category and related books are deleted. | Related books were cascade-deleted. | Pass |
| 18 | Undo category deletion | Category and books are restored. | Batch undo restored deleted records. | Pass |
| 19 | Sort books by title | Books appear alphabetically by title. | Sorted display was correct. | Pass |
| 20 | Sort books by borrow count | Books appear ordered by borrow count. | Sorted display was correct. | Pass |
| 21 | View history | Stack history is displayed. | Recent actions appeared in history table. | Pass |
| 22 | Undo add book | Added book is removed. | Book was removed after undo. | Pass |
| 23 | Undo delete book | Deleted book is restored. | Book was restored after undo. | Pass |
| 24 | Undo update book | Previous book data is restored. | Old data was restored after undo. | Pass |
| 25 | Undo borrow book | Quantity increases and borrow count decreases. | Borrow reversal worked correctly. | Pass |
| 26 | Undo return book | Quantity decreases again. | Return reversal worked correctly. | Pass |
| 27 | Undo auto-borrow | Student returns to front of queue. | Student was reinserted using `enqueueFront()`. | Pass |
| 28 | Exit and restart program | Data is loaded from files. | Books, queue, and categories loaded correctly. | Pass |

### 7.2 Edge Cases Tested

| Edge Case | Result |
|---|---|
| Search title with no matching result | System displayed a no-result message. |
| Search title with empty input | All books were displayed because an empty query matches every title. |
| Display books when list is empty | System displayed a friendly empty-message. |
| Sort books when list is empty | System avoided sorting and displayed a message. |
| Return a book when no student is waiting | Quantity increased normally. |
| Waiting queue contains students for different books | `dequeueByBookId()` skipped unrelated requests. |
| Delete category with zero books | Category was deleted and could be restored by undo. |
| Load missing file | Program started with an empty structure instead of crashing. |

### 7.3 Data Persistence Testing

The following files were checked after operations:

| File | Verification |
|---|---|
| `data/books.csv` | Book records were saved with ID, title, author, category, quantity, status, and borrow count. |
| `data/queue.txt` | Waiting students were saved with student ID, name, and requested book ID. |
| `data/categories.txt` | Categories were saved with borrow counts. |

The program successfully loaded saved data again after restart.

---

<div style="page-break-after: always;"></div>

## 8. Conclusion

The Smart Library Management System successfully demonstrates how multiple data structures and algorithms can be combined to solve a practical problem. The project provides useful library features such as book management, searching, sorting, borrowing, returning, waiting queue handling, category management, undo operations, and file persistence.

Each data structure has a clear role:

| Data Structure | Contribution |
|---|---|
| Linked List | Stores all book records dynamically. |
| Hash Table | Provides fast average-case book search by ID. |
| Queue | Manages students waiting for unavailable books. |
| Stack | Supports undo history for recent actions. |
| Binary Search Tree | Organizes categories alphabetically and stores borrow counts. |
| Array | Allows efficient sorting of book data for display. |

The project also shows the importance of synchronization between data structures. Since books are stored in both the Linked List and Hash Table, helper functions such as `syncAdd()`, `syncUpdate()`, and `syncDelete()` are used to keep the data consistent.

The undo feature is one of the strongest parts of the project. It uses a Stack to reverse operations and can also handle more advanced cases such as automatic borrowing and category cascade deletion. The waiting queue also supports realistic library behavior by allowing students to wait for unavailable books and receive them automatically when copies become available.

Overall, the system meets the main project requirements and provides a complete example of using Data Structures and Algorithms in a real application.

### 8.1 Future Improvements

Possible future improvements include:

- Add a graphical user interface.
- Add login roles for librarian and student users.
- Use a larger or dynamically resizing Hash Table.
- Balance the BST using an AVL Tree or Red-Black Tree.
- Add partial search by author and category.
- Persist undo history across program restarts.
- Improve CSV handling for titles or author names containing commas.
- Add automated unit tests for each data structure.

---

**End of Report**
