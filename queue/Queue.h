#ifndef QUEUE_H
#define QUEUE_H

#include "../utils/type.h"
struct QElement{
    Student data;
    QElement* next;
};
struct Queue{
    int size;
    QElement* front;
    QElement* rear;
    Queue();
    void enqueue(Student data);
    Student dequeue();
    Student peek();
    void display();
    bool isEmpty();
    int getSize();
    Student dequeueByBookId(int bookId);
};
#endif
