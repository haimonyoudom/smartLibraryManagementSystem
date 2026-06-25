#include "LinkedList.h"
#include <iostream>
using namespace std;

//helper
int LinkedList::getSize(){
    return size;
}
bool LinkedList::isEmpty(){
    return (head == nullptr);
}
LinkedList::LinkedList(){
    head = nullptr;
    tail = nullptr;
    size = 0;
}
//CRUD book
void LinkedList::addBook(Book data){
    Element* e = new Element;
    e->data = data;
    e->next = nullptr;
    if(isEmpty()) head = e;
    else tail->next = e;
    tail = e;
    size++;
}
void LinkedList::getallBook() {
    Element* e = head;

    printf("+----+---------------------------+---------------------------+---------------------------+-----+-----------+----------+\n");
    printf("| ID | Title                     | Author                    | Category                  | Qty | Status    | Borrowed |\n");
    printf("+----+---------------------------+---------------------------+---------------------------+-----+-----------+----------+\n");

    while (e != nullptr) {
        printf("| %-2d | %-25.25s | %-25.25s | %-25.25s | %-3d | %-9.9s | %-8d |\n",
               e->data.id,
               e->data.title.c_str(),
               e->data.author.c_str(),
               e->data.category.c_str(),
               e->data.quantity,
               e->data.status.c_str(),
               e->data.borrowCount);

        e = e->next;
    }

    printf("+----+---------------------------+---------------------------+---------------------------+-----+-----------+----------+\n");
}
bool LinkedList::updateBook(int bookId,Book updateBook){
    Element* e = head;
    while (e != nullptr){
        if(e->data.id == bookId){
            e->data = updateBook;
            return true;
        }
        e = e->next;
    }
    return false;
}
bool LinkedList::deleteBook(int bookId)
{
    if (head == nullptr)
        return false;

    // Delete head
    if (head->data.id == bookId)
    {
        Element* temp = head;
        head = head->next;

        if (head == nullptr) // list became empty
            tail = nullptr;

        delete temp;
        size--;
        return true;
    }

    Element* prev = head;
    Element* curr = head->next;

    while (curr != nullptr)
    {
        if (curr->data.id == bookId)
        {
            prev->next = curr->next;

            // deleting tail
            if (curr == tail)
                tail = prev;

            delete curr;
            size--;
            return true;
        }

        prev = curr;
        curr = curr->next;
    }

    return false; // not found
}
Element* LinkedList::searchById(int bookId){
    Element* e = head;
    while (e != nullptr)
    {
        if(e->data.id == bookId){
            return e;
        }
        e = e->next;
    }
    return nullptr;
}
Element* LinkedList::searchByTitle(string title){
    Element* e = head;
    while(e != nullptr){
        if(e->data.title == title){
            return e;
        }
        e = e->next;
    }
    return nullptr;
}
