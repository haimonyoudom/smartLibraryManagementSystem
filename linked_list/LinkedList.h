#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "../util/type.h"


struct Element{
    Book data;
    Element* next;
};
struct LinkedList
{
    int size;
    Element* head;
    Element* tail;
    LinkedList();
    //crud operation
    void addBook(Book data);
    bool deleteBook(int bookId);
    bool updateBook(int bookId,Book updateBook);
    void getallBook();
    //util
    bool isEmpty();
    int getSize();
    //searching data
    Element* searchById(int bookId);
    Element* searchByTitle(string title);
};

#endif