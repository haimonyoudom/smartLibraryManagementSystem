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
    void mostBorrowedRec(TreeNode* node, string& category, int& maxCount);
    void saveRec(TreeNode* node, ostream& out);

public:
    BST();
    ~BST();

    void addCategory(string category);
    void deleteCategory(string category);
    void displayTree();
    bool searchCategory(string category);
    void incrementCount(string category);
    string getMostBorrowedCategory();
    void saveCategories(ostream& out);
};

#endif
