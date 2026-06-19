#ifndef STACK_H
#define STACK_H

#include "./util/type.h"

struct StackElement
{
    Action data;
    StackElement* next;
};
struct Stack
{
    StackElement* top;
    int size;
    Stack();
    void push(Action data);
    Action pop();
    Action peek();
    void viewHistory();
    bool isEmpty();
    int getSize();
};
#endif