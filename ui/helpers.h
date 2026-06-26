#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include <string>
#include "../utils/type.h"
#include "../linked_list/LinkedList.h"
#include "../hash_table/hashTable.h"
#include "../utils/FileIO.h"
#include "../queue/Queue.h"
#include "../tree/tree.h"
using namespace std;

// ── Input ─────────────────────────────────────
void   clearInput();
int    readInt(const string& label);
string readLine(const string& label);
string readLineDefault(const string& label, const string& oldValue);
int    readIntDefault(const string& label, int oldValue);
bool   confirm(const string& message);
void   pause();

// ── Display ───────────────────────────────────
void printDivider();
void printHeader();
void printBookRow(const Book& b);
void printBook(const Book& b);

// ── Book logic ────────────────────────────────
void updateBookStatus(Book& b);
void saveAll(LinkedList& books, Queue& queue, BST& categories);
void syncAdd(LinkedList& books, HashTable& ht, const Book& b);
void syncUpdate(LinkedList& books, HashTable& ht, int id, const Book& updated);
void syncDelete(LinkedList& books, HashTable& ht, int id);
void buildIndex(LinkedList& books, HashTable& ht);

// ── Category selection (shared by Book + Category UI) ──
string selectCategory(BST& categories, const string& current = "");

#endif