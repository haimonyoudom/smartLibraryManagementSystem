#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
using namespace std;

struct TreeNode {
    string category;
    int bookCount;
    TreeNode* left;
    TreeNode* right;
};

class BST {
private:
    TreeNode* root;

    TreeNode* insertRec(TreeNode* node, string cat);
    TreeNode* deleteRec(TreeNode* node, string cat);
    TreeNode* findMin(TreeNode* node);
    void inorderRec(TreeNode* node);
    void destroyRec(TreeNode* node);
    bool searchRec(TreeNode* node, string cat);
    void incrementRec(TreeNode* node, string cat);
    void setCountRec(TreeNode* node, string cat, int count);
    void mostBorrowedRec(TreeNode* node, string& category, int& maxCount);
    void saveRec(TreeNode* node, ostream& out);
    void displayNumberedRec(TreeNode* node, int& number);
    string getByIndexRec(TreeNode* node, int target, int& number);
    int countRec(TreeNode* node);

public:
    BST();
    ~BST();

    void clear();
    void addCategory(string category);
    void deleteCategory(string category);
    void displayTree();
    void displayCategoriesNumbered();
    bool searchCategory(string category);
    void incrementCount(string category);
    void setCount(string category, int count);
    string getMostBorrowedCategory();
    string getCategoryByIndex(int index);
    int countCategories();
    void saveCategories(ostream& out);
};

#endif
