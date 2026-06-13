#ifndef TYPE_H
#define TYPE_H

#include <string>
using namespace std;
struct Book
{
    int id;
    string title;
    string author;
    string category;
    int quantity;
    string status;
    int borrowCount;

};

struct Student
{
    int studentId;
    string name;
    int requestedBookId;
};
struct Action
{
    string type;
    Book bookSnapshot;
};


#endif