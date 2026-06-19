#include "Sort.H"
#include <iostream>
using namespace std;

class Sort {
public:
    // O(n²) sorts
    static void bubbleSort(Book arr[], int n, string field);
    static void insertionSort(Book arr[], int n, string field);

    // O(n log n) sorts
    static void mergeSort(Book arr[], int left, int right, string field);
    static void quickSort(Book arr[], int low, int high, string field);

private:
    // Helpers
    static void   merge(Book arr[], int l, int m, int r, string field);
    static int    partition(Book arr[], int low, int high, string field);
    static bool   compare(Book& a, Book& b, string field);
    static void   swap(Book& a, Book& b);
    // Convert linked list to array for sorting
    static Book*  listToArray(Node* head, int size);
};