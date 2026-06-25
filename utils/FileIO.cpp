#include "FileIO.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

// split a CSV line into fields by comma.
static void splitCSV(const string& line, string fields[], int count) {
    stringstream ss(line);
    string token;
    for (int i = 0; i < count; i++) {
        getline(ss, token, ',');
        fields[i] = token;
    }
}

// BOOKS -> books.csv
void saveBooksToCSV(LinkedList& list, string filename) {
    ofstream fout(filename);
    if (!fout) {
        cout << "Error: could not open " << filename << " for writing.\n";
        return;
    }

    Element* curr = list.head;
    while (curr != nullptr) {
        curr->data.status = (curr->data.quantity > 0) ? "available" : "borrowed";
        Book b = curr->data;
        fout << b.id << ","
             << b.title << ","
             << b.author << ","
             << b.category << ","
             << b.quantity << ","
             << b.status << ","
             << b.borrowCount << "\n";
        curr = curr->next;
    }

    fout.close();
}

void loadBooksFromCSV(LinkedList& list, string filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "No existing " << filename << " found. Starting with an empty list.\n";
        return;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        string fields[7];
        splitCSV(line, fields, 7);

        Book b;
        b.id          = stoi(fields[0]);
        b.title       = fields[1];
        b.author      = fields[2];
        b.category    = fields[3];
        b.quantity    = stoi(fields[4]);
        b.status      = (b.quantity > 0) ? "available" : "borrowed";
        b.borrowCount = stoi(fields[6]);

        list.addBook(b);
    }

    fin.close();
}

// WAITING QUEUE -> queue.txt
void saveQueueToFile(Queue& q, string filename) {
    ofstream fout(filename);
    if (!fout) {
        cout << "Error: could not open " << filename << " for writing.\n";
        return;
    }

    int n = q.getSize();
    Student* temp = new Student[n];

    for (int i = 0; i < n; i++) {
        temp[i] = q.dequeue();
        fout << temp[i].studentId << ","
             << temp[i].name << ","
             << temp[i].requestedBookId << "\n";
    }
    for (int i = 0; i < n; i++) {
        q.enqueue(temp[i]);
    }

    delete[] temp;
    fout.close();
}

void loadQueueFromFile(Queue& q, string filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "No existing " << filename << " found. Starting with an empty queue.\n";
        return;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        string fields[3];
        splitCSV(line, fields, 3);

        Student s;
        s.studentId       = stoi(fields[0]);
        s.name             = fields[1];
        s.requestedBookId = stoi(fields[2]);

        q.enqueue(s);
    }

    fin.close();
}

// CATEGORIES -> categories.txt
void saveCategoriesToFile(BST& tree, string filename) {
    ofstream fout(filename);
    if (!fout) {
        cout << "Error: could not open " << filename << " for writing.\n";
        return;
    }

    tree.saveCategories(fout);
    fout.close();
}

void loadCategoriesFromFile(BST& tree, string filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "No existing " << filename << " found. Starting with an empty tree.\n";
        return;
    }

    string category;
    while (getline(fin, category)) {
        if (category.empty()) continue;
        tree.addCategory(category);
    }

    fin.close();
}
