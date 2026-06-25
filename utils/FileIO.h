#ifndef FILEIO_H
#define FILEIO_H

#include "../utils/type.h"
#include "../linked_list/LinkedList.h"
#include "../queue/Queue.h"
#include "../tree/tree.h"

// books.csv  ->  id,title,author,category,qty,status,borrowCount
void saveBooksToCSV(LinkedList& list, string filename = "data/books.csv");
void loadBooksFromCSV(LinkedList& list, string filename = "data/books.csv");

// queue.txt -> one student per line
void saveQueueToFile(Queue& q, string filename = "data/queue.txt");
void loadQueueFromFile(Queue& q, string filename = "data/queue.txt");

// categories.txt -> one category per line
void saveCategoriesToFile(BST& tree, string filename = "data/categories.txt");
void loadCategoriesFromFile(BST& tree, string filename = "data/categories.txt");

#endif
