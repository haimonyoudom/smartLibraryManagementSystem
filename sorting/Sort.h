#ifndef SORT_H
#define SORT_H
#include "../utils/type.h"
#include "../linked_list/LinkedList.h"

class Sort {
public:
    // O(n^2) 
    static void bubbleSort(Book arr[], int n, string field);
    static void insertionSort(Book arr[], int n, string field);

    // O(n log n) 
    static void mergeSort(Book arr[], int lb, int ub, string field);
    static void quickSort(Book arr[], int lb, int ub, string field);

    static Book* listToArray(Element* head, int size);

private:
    // Helpers
    static void merge(Book arr[], int lb, int mid, int ub, string field);
    static int  partition(Book arr[], int lb, int ub, string field);

    static bool compare(Book& a, Book& b, string field);
    static void swapBooks(Book& a, Book& b);
};

#endif
