#include "Sort.h"
#include <iostream>
using namespace std;

bool Sort::compare(Book& a, Book& b, string field) {
    if (field == "id")          
        return a.id > b.id;
    if (field == "title")       
        return a.title > b.title;
    if (field == "author")      
        return a.author > b.author;
    if (field == "category")    
        return a.category > b.category;
    if (field == "quantity")    
        return a.quantity > b.quantity;
    if (field == "borrowCount") 
        return a.borrowCount > b.borrowCount;

    return a.id > b.id; // default: sort by ID
}

void Sort::swapBooks(Book& a, Book& b) {
    Book temp = a;
    a = b;
    b = temp;
}

// Bubble sort by Book ID
void Sort::bubbleSort(Book arr[], int n, string field) {
    for (int i=0; i<n-1; i++) {
        bool flag = false;
        for (int j=0; j<n-i-1; j++) {
            if (compare(arr[j], arr[j+1], field)) {  // arr[j] > arr[j+1]
                swapBooks(arr[j], arr[j+1]);
                flag = true;
            }
        }
        if (!flag) break;
    }
}

// Insertion sort by Title or Author
void Sort::insertionSort(Book arr[], int n, string field) {
    for (int i = 1; i < n; i++) {
        Book key = arr[i];
        int j = i - 1;
        while (j >= 0 && compare(arr[j], key, field)) { // arr[j] > key
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Merge sort by Quantity
void Sort::merge(Book arr[], int lb, int mid, int ub, string field) {
    int i = lb;
    int j = mid + 1;
    int k = 0;
    int n = ub - lb + 1;
    Book* b = new Book[n];

    while (i <= mid && j <= ub) {
        if (!compare(arr[i], arr[j], field)) { // arr[i] <= arr[j]
            b[k++] = arr[i++];
        } else {
            b[k++] = arr[j++];
        }
    }
    while (i <= mid) {           
        b[k++] = arr[i++];
    }
    while (j <= ub) {            
        b[k++] = arr[j++];
    }
    for (k = 0; k < n; k++) {    
        arr[lb + k] = b[k];
    }
    delete[] b;
}

void Sort::mergeSort(Book arr[], int lb, int ub, string field) {
    if (lb < ub) {                       // base case: subarray has >= 2 elements
        int mid = (lb + ub) / 2;
        mergeSort(arr, lb, mid, field);      // sort left half
        mergeSort(arr, mid + 1, ub, field);  // sort right half
        merge(arr, lb, mid, ub, field);      // merge the two sorted halves
    }
}

// Quick sort by any field
int Sort::partition(Book arr[], int lb, int ub, string field) {
    Book pivot = arr[lb];   // pivot = first element
    int start = lb;
    int end = ub;

    while (start < end) {
        while (!compare(arr[start], pivot, field) && start <= ub) { // arr[start] <= pivot
            start++;
        }
        while (compare(arr[end], pivot, field) && end >= lb) {      // arr[end] > pivot
            end--;
        }
        if (start < end) {
            swapBooks(arr[start], arr[end]);
        }
    }
    swapBooks(arr[lb], arr[end]); 
    return end;
}

void Sort::quickSort(Book arr[], int lb, int ub, string field) {
    if (lb < ub) {
        int pos = partition(arr, lb, ub, field);
        quickSort(arr, lb, pos - 1, field);
        quickSort(arr, pos + 1, ub, field);
    }
}

// Convert linked list to array
Book* Sort::listToArray(Element* head, int size) {
    Book* arr = new Book[size];
    Element* current = head;
    int i = 0;
    while (current != nullptr && i < size) {
        arr[i] = current->data;
        current = current->next;
        i++;
    }
    return arr;
}
