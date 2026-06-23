#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../util/type.h"

const int HT_SIZE = 10;

struct HTNode {
    Book data;
    HTNode* next;
};

struct HTBucket {
    int n;
    HTNode* head;
    HTNode* tail;
};

struct HashTable {
    HTBucket* table[HT_SIZE];
    HashTable();
    void insertBook(Book data);
    bool deleteBook(int bookId);
    Book* searchById(int bookId);
    void display();
};

#endif