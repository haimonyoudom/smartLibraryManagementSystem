#include "HashTable.h"
#include <iostream>
using namespace std;

int hashFunction(int key) {
    return key % HT_SIZE;
}

HashTable::HashTable() {
    for (int i = 0; i < HT_SIZE; i++) {
        table[i] = new HTBucket;
        table[i]->n = 0;
        table[i]->head = nullptr;
        table[i]->tail = nullptr;
    }
}

void HashTable::insertBook(Book data) {
    int index = hashFunction(data.id);
    HTNode* node = new HTNode;
    node->data = data;
    node->next = nullptr;
    if (table[index]->n == 0)
        table[index]->head = table[index]->tail = node;
    else {
        table[index]->tail->next = node;
        table[index]->tail = node;
    }
    table[index]->n++;
}

Book* HashTable::searchById(int bookId) {
    int index = hashFunction(bookId);
    HTNode* e = table[index]->head;
    while (e != nullptr) {
        if (e->data.id == bookId)
            return &e->data;
        e = e->next;
    }
    return nullptr;
}

bool HashTable::deleteBook(int bookId) {
    int index = hashFunction(bookId);
    if (table[index]->n == 0) return false;

    HTNode* e = table[index]->head;
    HTNode* prev = nullptr;

    while (e != nullptr) {
        if (e->data.id == bookId) {
            if (prev == nullptr) {
                table[index]->head = e->next;
                if (table[index]->head == nullptr)
                    table[index]->tail = nullptr;
            } else {
                prev->next = e->next;
                if (e == table[index]->tail)
                    table[index]->tail = prev;
            }
            delete e;
            table[index]->n--;
            return true;
        }
        prev = e;
        e = e->next;
    }
    return false;
}