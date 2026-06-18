#include <iostream>
#include "Queue.h"

using namespace std;

int main() {
    Queue q;

    Student s1 = {101, "John Smith", 1};
    Student s2 = {102, "Alice Brown", 3};
    Student s3 = {103, "David Lee", 5};

    cout << "Adding students to queue...\n";

    q.enqueue(s1);
    q.enqueue(s2);
    q.enqueue(s3);

    cout << "\nCurrent Queue:\n";
    q.display();

    cout << "\nFront Student:\n";
    Student front = q.peek();
    cout << "ID: " << front.studentId
         << ", Name: " << front.name
         << ", Requested Book: " << front.requestedBookId
         << endl;

    cout << "\nDequeued Student:\n";
    Student removed = q.dequeue();
    cout << "ID: " << removed.studentId
         << ", Name: " << removed.name
         << ", Requested Book: " << removed.requestedBookId
         << endl;

    cout << "\nQueue After Dequeue:\n";
    q.display();

    cout << "\nQueue Size: " << q.getSize() << endl;

    return 0;
}