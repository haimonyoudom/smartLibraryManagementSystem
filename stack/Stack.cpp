#include "Stack.h"
#include <iostream>
using namespace std;

Stack::Stack(){
    top = nullptr;
    size = 0;
}
bool Stack::isEmpty(){
    return top == nullptr;
}
int Stack::getSize(){
    return size;
}
//LIFO Last In First Out
void Stack::push(Action data){
    StackElement* e = new StackElement;
    e->data = data;
    e->next = top;

    top = e;
    size++;
}
Action Stack::pop(){
    if(isEmpty()){
        return Action();
    }
    StackElement* e = top;
    Action data = e->data;
    top = e->next;
    delete e;
    size--;
    return data;
}
Action Stack::peek(){
    if(isEmpty()){
        return Action();
    }
    return top->data;
}
void Stack::viewHistory() {
    if(isEmpty()){
        cout << "No history available.\n";
        return;
    }

    StackElement* e = top;

    printf("+--------+----------------------+----------------------+----------------+----------+------------+--------------+\n");
    printf("| ID     | Title                | Author               | Category       | Quantity | Status     | Action       |\n");
    printf("+--------+----------------------+----------------------+----------------+----------+------------+--------------+\n");

    while(e != nullptr){
        printf("| %-6d | %-20.20s | %-20.20s | %-14.14s | %-8d | %-10.10s | %-12.12s |\n",
               e->data.bookSnapshot.id,
               e->data.bookSnapshot.title.c_str(),
               e->data.bookSnapshot.author.c_str(),
               e->data.bookSnapshot.category.c_str(),
               e->data.bookSnapshot.quantity,
               e->data.bookSnapshot.status.c_str(),
               e->data.type.c_str());

        e = e->next;
    }

    printf("+--------+----------------------+----------------------+----------------+----------+------------+--------------+\n");
}
