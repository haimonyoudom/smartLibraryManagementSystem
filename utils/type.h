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
struct Action {
    string type;
    Book   bookSnapshot;
    string meta;// carries category name for CAT_DELETE_START
    Student studentSnapshot; 
    bool    hasStudent = false; 
};

#endif