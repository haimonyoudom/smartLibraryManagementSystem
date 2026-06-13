# Smart Library Management System
### DSA Project — C++ Implementation Guide
**Deadline:** 28 June 2026 | **Language:** C++ (console-based)

---

## Project Structure

```
SmartLibrary/
├── main.cpp
├── README.md
│
├── linkedlist/
│   ├── LinkedList.h
│   └── LinkedList.cpp
│
├── stack/
│   ├── Stack.h
│   └── Stack.cpp
│
├── queue/
│   ├── Queue.h
│   └── Queue.cpp
│
├── hashtable/
│   ├── HashTable.h
│   └── HashTable.cpp
│
├── tree/
│   ├── Tree.h
│   └── Tree.cpp
│
├── sorting/
│   ├── Sort.h
│   └── Sort.cpp
│
├── utils/
│   ├── FileIO.h
│   └── FileIO.cpp
│
└── data/
    ├── books.csv
    ├── queue.txt
    └── categories.txt
```

---

## Data Structures & Shared Types

Define these in a shared `types.h` included by all modules.

```cpp
// types.h
struct Book {
    int     id;
    string  title;
    string  author;
    string  category;
    int     quantity;
    string  status;     // "available" | "borrowed"
    int     borrowCount; // for statistics
};

struct Student {
    int    studentId;
    string name;
    int    requestedBookId;
};

struct Action {
    string type;        // "ADD" | "DELETE" | "UPDATE" | "BORROW"
    Book   bookSnapshot; // state of book before the action
};
```

---

## Module 1 — Book Management (Linked List)

**File:** `linkedlist/LinkedList.h` + `LinkedList.cpp`

### Node & List

```cpp
// linkedlist/LinkedList.h
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "../types.h"

struct Node {
    Book data;
    Node* next;
};

struct LinkedList {
    Node* head;
    int size;
};

// CRUD Operations
void addBook(LinkedList& list, Book b);
bool deleteBook(LinkedList& list, int bookId);
bool updateBook(LinkedList& list, int bookId, Book updatedData);
void displayAll(LinkedList& list);

// Search Operations
Node* searchById(LinkedList& list, int bookId);
Node* searchByTitle(LinkedList& list, string title);

// Utilities
int getSize(LinkedList& list);
bool isEmpty(LinkedList& list);

#endif
```



### Header / Source File Pattern

Use **structs + standalone functions** instead of classes.

**Header file (`.h`)**
```cpp
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "../types.h"

struct Node {
    Book data;
    Node* next;
};

struct LinkedList {
    Node* head;
    int size;
};

void addBook(LinkedList& list, Book b);

#endif
```

**Source file (`.cpp`)**
```cpp
#include "LinkedList.h"
#include <iostream>
using namespace std;

void addBook(LinkedList& list, Book b) {
    // implementation here
}
```

**Usage in `main.cpp`**
```cpp
#include "linkedlist/LinkedList.h"
#include "stack/Stack.h"
#include "queue/Queue.h"

int main() {
    LinkedList books = {nullptr, 0};

    Book b;
    addBook(books, b);

    return 0;
}
```

### Functions to Implement

| Function | What it does |
|---|---|
| `addBook(Book b)` | Create new Node, append to tail (or insert sorted). O(n). |
| `deleteBook(int id)` | Traverse to find node, unlink and free it. O(n). |
| `updateBook(int id, Book data)` | Find node by ID, overwrite fields. O(n). |
| `displayAll()` | Traverse from head, print each Book. O(n). |
| `searchById(int id)` | Traverse and compare IDs, return pointer. O(n). |
| `searchByTitle(string t)` | Traverse and compare titles (case-insensitive). O(n). |

### Key Logic Notes
- Keep a `tail` pointer so `addBook` is O(1) instead of O(n).
- `deleteBook` needs a `prev` pointer to re-link the list.
- When a book's `quantity` drops to 0, set `status = "borrowed"`.

---

## Module 2 — Borrowing Waitlist (Queue)

**File:** `queue/Queue.h` + `Queue.cpp`

### Queue Structure

```cpp
struct QNode {
    Student data;
    QNode*  next;
};

class Queue {
private:
    QNode* front;
    QNode* rear;
    int    size;
public:
    Queue();
    ~Queue();
    void    enqueue(Student s);
    Student dequeue();
    Student peek();
    void    display();
    bool    isEmpty();
    int     getSize();
};
```

### Functions to Implement

| Function | What it does |
|---|---|
| `enqueue(Student s)` | Add student to rear when requested book is unavailable. O(1). |
| `dequeue()` | Remove from front when book becomes available. Return Student. O(1). |
| `peek()` | Return front student without removing. O(1). |
| `display()` | Print all waiting students in order. O(n). |

### Key Logic Notes
- Call `enqueue` from the borrow menu when `book.quantity == 0`.
- Call `dequeue` from the return menu — auto-assign the returned copy.
- Display queue alongside book info so librarian knows who is next.

---

## Module 3 — Undo Operations (Stack)

**File:** `stack/Stack.h` + `Stack.cpp`

### Stack Structure

```cpp
struct StackNode {
    Action   data;
    StackNode* next;
};

class Stack {
private:
    StackNode* top;
    int        size;
public:
    Stack();
    ~Stack();
    void   push(Action a);
    Action pop();
    Action peek();
    void   viewHistory();
    bool   isEmpty();
    int    getSize();
};
```

### Functions to Implement

| Function | What it does |
|---|---|
| `push(Action a)` | Save action + book snapshot before any change. O(1). |
| `pop()` | Remove top action and return it for undo. O(1). |
| `peek()` | See last action without undoing. O(1). |
| `viewHistory()` | Print all actions from top to bottom. O(n). |

### Key Logic Notes
- Push **before** modifying data so snapshot holds the old state.
- Undo for `ADD` → call `deleteBook`. Undo for `DELETE` → call `addBook` with snapshot.
- Limit stack depth to ~50 entries (optional) to avoid memory bloat.

---

## Module 4 — Fast Book Search (Hash Table)

**File:** `hashtable/HashTable.h` + `HashTable.cpp`

### Hash Table Structure

```cpp
struct HTNode {
    int     key;     // book ID
    Book    value;
    HTNode* next;    // chaining for collisions
};

class HashTable {
private:
    static const int TABLE_SIZE = 101; // prime number
    HTNode* table[TABLE_SIZE];
    int     hashFunction(int key);
public:
    HashTable();
    ~HashTable();
    void  insert(Book b);
    Book* search(int bookId);
    bool  remove(int bookId);
    void  display();
};
```

### Functions to Implement

| Function | What it does |
|---|---|
| `hashFunction(int key)` | `return key % TABLE_SIZE`. O(1). |
| `insert(Book b)` | Hash the ID, prepend node to bucket chain. O(1) avg. |
| `search(int id)` | Hash ID, traverse bucket chain to find match. O(1) avg. |
| `remove(int id)` | Hash ID, unlink node from chain. O(1) avg. |
| `display()` | Print all non-empty buckets with their entries. O(n). |

### Key Logic Notes
- Use `TABLE_SIZE = 101` (a prime) to distribute keys evenly.
- Each bucket is a mini linked list (chaining strategy).
- Keep HashTable in sync with LinkedList — insert/delete both when adding/removing books.

---

## Module 5 — Category Management (Tree)

**File:** `tree/Tree.h` + `Tree.cpp`

### Tree Structure (Binary Search Tree by category name)

```cpp
struct TreeNode {
    string   category;
    int      bookCount;  // how many books in this category
    TreeNode* left;
    TreeNode* right;
};

class BST {
private:
    TreeNode* root;
    TreeNode* insertRec(TreeNode* node, string cat);
    TreeNode* deleteRec(TreeNode* node, string cat);
    TreeNode* findMin(TreeNode* node);
    void      inorderRec(TreeNode* node);
    void      destroyRec(TreeNode* node);
public:
    BST();
    ~BST();
    void     addCategory(string category);
    void     deleteCategory(string category);
    void     displayTree();
    bool     searchCategory(string category);
    void     incrementCount(string category);
    string   getMostBorrowedCategory();
};
```

### Functions to Implement

| Function | What it does |
|---|---|
| `addCategory(string cat)` | Insert node into BST by alphabetical order. O(log n) avg. |
| `deleteCategory(string cat)` | BST delete with 3 cases (no child, one child, two children). O(log n) avg. |
| `displayTree()` | In-order traversal = sorted alphabetical output. O(n). |
| `searchCategory(string cat)` | BST search, return true/false. O(log n) avg. |
| `incrementCount(string cat)` | Find node, increment bookCount. O(log n) avg. |
| `getMostBorrowedCategory()` | Traverse all nodes, return category with highest count. O(n). |

### Key Logic Notes
- Call `addCategory` when a new category is first used in a book.
- Call `incrementCount` when a book in that category is borrowed.
- For display, in-order traversal gives alphabetical output automatically.

---

## Module 6 — Sorting Books (Sorting Algorithms)

**File:** `sorting/Sort.h` + `Sort.cpp`

```cpp
class Sort {
public:
    // O(n²) sorts
    static void bubbleSort(Book arr[], int n, string field);
    static void insertionSort(Book arr[], int n, string field);

    // O(n log n) sorts
    static void mergeSort(Book arr[], int left, int right, string field);
    static void quickSort(Book arr[], int low, int high, string field);

private:
    // Helpers
    static void   merge(Book arr[], int l, int m, int r, string field);
    static int    partition(Book arr[], int low, int high, string field);
    static bool   compare(Book& a, Book& b, string field);
    static void   swap(Book& a, Book& b);
    // Convert linked list to array for sorting
    static Book*  listToArray(Node* head, int size);
};
```

### Functions to Implement

| Function | Sort by | Complexity |
|---|---|---|
| `bubbleSort` | Book ID | O(n²) time, O(1) space |
| `insertionSort` | Title or Author | O(n²) time, O(1) space |
| `mergeSort` | Quantity | O(n log n) time, O(n) space |
| `quickSort` | Any field | O(n log n) avg, O(log n) space |

### Key Logic Notes
- Convert the linked list to a `Book[]` array before sorting, sort in place, then redisplay.
- The `compare(a, b, field)` helper compares two books on the chosen field — lets you reuse one sort for multiple fields.
- For the report: clearly document the Big O of each and explain when to use which.

---

## Module 7 — Statistics Dashboard

No separate class needed — call methods from other modules.

### Functions to Implement (in `main.cpp` or a `Stats` helper)

| Function | Uses | Output |
|---|---|---|
| `getTotalBooks()` | `LinkedList::getSize()` | Integer count |
| `getAvailableBooks()` | Traverse LinkedList, count `status == "available"` | Integer count |
| `getBorrowedBooks()` | Traverse LinkedList, count `status == "borrowed"` | Integer count |
| `getMostBorrowedCategory()` | `BST::getMostBorrowedCategory()` | Category name string |
| `displayDashboard()` | All of the above | Formatted console output |

### Sample Output
```
╔══════════════════════════════╗
║   Library Statistics         ║
╠══════════════════════════════╣
║ Total Books       :  120     ║
║ Available         :   95     ║
║ Borrowed          :   25     ║
║ Most Borrowed Cat : Science  ║
╚══════════════════════════════╝
```

---

## File I/O

**File:** `utils/FileIO.h` + `FileIO.cpp`

### Functions to Implement

| Function | File | Format |
|---|---|---|
| `saveBooksToCSV(LinkedList& list)` | `data/books.csv` | `id,title,author,category,qty,status,borrowCount` |
| `loadBooksFromCSV(LinkedList& list)` | `data/books.csv` | Parse each line, call `addBook` |
| `saveQueueToFile(Queue& q)` | `data/queue.txt` | One student per line |
| `loadQueueFromFile(Queue& q)` | `data/queue.txt` | Parse and enqueue |
| `saveCategoriesToFile(BST& tree)` | `data/categories.txt` | One category per line |
| `loadCategoriesFromFile(BST& tree)` | `data/categories.txt` | Parse and addCategory |

### Key Logic Notes
- Call all `load*` functions at the **start** of `main()`.
- Call all `save*` functions just before `return 0` in `main()`.
- Use `fstream` — no external libraries needed.

---

## Main Menu Structure (`main.cpp`)

```
===== Smart Library Management System =====

1. Book Management (Linked List)
   1.1  Add book
   1.2  Delete book
   1.3  Update book
   1.4  Display all books
   1.5  Search book (by ID or title)

2. Borrowing Queue
   2.1  Borrow a book (enqueue if unavailable)
   2.2  Return a book (dequeue next student)
   2.3  Display waiting queue

3. Undo Operations (Stack)
   3.1  Undo last action
   3.2  View action history

4. Fast Search (Hash Table)
   4.1  Search book by ID

5. Category Management (Tree)
   5.1  Add category
   5.2  Delete category
   5.3  Display all categories

6. Sort Books
   6.1  Sort by Book ID (Bubble Sort)
   6.2  Sort by Title (Insertion Sort)
   6.3  Sort by Quantity (Merge Sort)

7. Statistics Dashboard

0. Save & Exit
```

---

## Complexity Summary (for Report)

| Module | Operation | Time Complexity | Space Complexity |
|---|---|---|---|
| Linked List | Add / Delete / Search | O(n) | O(1) |
| Queue | Enqueue / Dequeue | O(1) | O(1) |
| Stack | Push / Pop | O(1) | O(1) |
| Hash Table | Insert / Search / Delete | O(1) avg, O(n) worst | O(n) |
| BST | Insert / Search / Delete | O(log n) avg, O(n) worst | O(log n) |
| Bubble Sort | Sort | O(n²) | O(1) |
| Insertion Sort | Sort | O(n²) | O(1) |
| Merge Sort | Sort | O(n log n) | O(n) |
| Quick Sort | Sort | O(n log n) avg | O(log n) |

---

## Team Member Responsibility Split (Suggested)

| Member | Modules | Files |
|---|---|---|
| Member 1 | Linked List + File I/O | `LinkedList.h/cpp`, `FileIO.h/cpp` |
| Member 2 | Hash Table + Sorting | `HashTable.h/cpp`, `Sort.h/cpp` |
| Member 3 | Stack + Queue | `Stack.h/cpp`, `Queue.h/cpp` |
| Member 4 | Tree + Stats + Main Menu | `Tree.h/cpp`, `main.cpp` |

> Each member integrates their module into `main.cpp` via their section of the menu.

---

## Weekly Progress Checklist

### Week 1 (by ~21 June)
- [ ] Repository created on GitHub, branches set up per member
- [ ] `types.h` agreed and committed
- [ ] Linked List: all CRUD functions working
- [ ] Stack: push/pop/undo working
- [ ] Queue: enqueue/dequeue working
- [ ] Hash Table: insert/search working
- [ ] Tree: add/display working
- [ ] Sorting: at least 2 algorithms working

### Week 2 (by 28 June)
- [ ] All modules integrated into main menu
- [ ] File I/O: save and load working for all data
- [ ] Statistics dashboard complete
- [ ] All 4 sorting algorithms complete with field selection
- [ ] Code cleaned: comments, headers, consistent style
- [ ] Project report written
- [ ] Presentation recorded (15–20 min, all members)
- [ ] GitHub repo finalized and link submitted