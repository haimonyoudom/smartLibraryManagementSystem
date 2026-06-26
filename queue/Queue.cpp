#include "Queue.h"
#include <iostream>
using namespace std;

int Queue::getSize(){
    return size;
}
Queue::Queue(){
    size =0;
    front = nullptr;
    rear = nullptr;
}
bool Queue::isEmpty(){
    return front == nullptr;
}
//FIFO First In First Out
void Queue::enqueue(Student data){
    QElement* q = new QElement;
    q->data = data;
    q->next = nullptr;
    if(isEmpty()) front = q;
    else rear->next = q;
    rear = q;
    size++; 
}
Student Queue::dequeue(){
    if(isEmpty()){
        cout << "Queue is empty\n";
        return Student();  
    }

    QElement* temp = front;
    Student data = front->data;

    front = front->next;

    if(front == nullptr)
        rear = nullptr;

    delete temp;
    size--;

    return data;
}
Student Queue::peek(){
    if(isEmpty()){
        cout << "Queue is empty\n";
        return Student();
    }

    return front->data;
}
Student Queue::dequeueByBookId(int bookId) {
    if (isEmpty()) return Student();

    QElement* curr = front;
    QElement* prev = nullptr;

    while (curr != nullptr) {
        if (curr->data.requestedBookId == bookId) {
            Student data = curr->data;

            if (prev == nullptr)        // removing front
                front = curr->next;
            else
                prev->next = curr->next;

            if (curr == rear)           // removing rear
                rear = prev;

            delete curr;
            size--;
            return data;
        }
        prev = curr;
        curr = curr->next;
    }

    return Student();   // not found
}
void Queue::display() {
    QElement* e = front;

    printf("+------------+---------------------------+-------------------+\n");
    printf("| Student ID | Name                      | Requested Book ID |\n");
    printf("+------------+---------------------------+-------------------+\n");

    while (e != nullptr) {
        printf("| %-10d | %-25.25s | %-17d |\n",
               e->data.studentId,
               e->data.name.c_str(),
               e->data.requestedBookId);

        e = e->next;
    }

    printf("+------------+---------------------------+-------------------+\n");
}