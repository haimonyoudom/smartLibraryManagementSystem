#include "tree.h"
#include <iostream>
using namespace std;

BST::BST() {
    root = nullptr;
}

BST::~BST() {
    destroyRec(root);
}

TreeNode* BST::insertRec(TreeNode* node, string cat) {
    if (node == nullptr) {
        TreeNode* temp = new TreeNode;
        temp->category = cat;
        temp->bookCount = 0;
        temp->left = nullptr;
        temp->right = nullptr;
        return temp;
    }

    if (cat < node->category) {
        node->left = insertRec(node->left, cat);
    } else if (cat > node->category) {
        node->right = insertRec(node->right, cat);
    }

    return node;
}

TreeNode* BST::findMin(TreeNode* node) {
    while (node != nullptr && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

TreeNode* BST::deleteRec(TreeNode* node, string cat) {
    if (node == nullptr) {
        return nullptr;
    }

    if (cat < node->category) {
        node->left = deleteRec(node->left, cat);
    } else if (cat > node->category) {
        node->right = deleteRec(node->right, cat);
    } else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }

        if (node->left == nullptr) {
            TreeNode* temp = node->right;
            delete node;
            return temp;
        }

        if (node->right == nullptr) {
            TreeNode* temp = node->left;
            delete node;
            return temp;
        }

        TreeNode* successor = findMin(node->right);
        node->category = successor->category;
        node->bookCount = successor->bookCount;
        node->right = deleteRec(node->right, successor->category);
    }

    return node;
}

void BST::inorderRec(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    inorderRec(node->left);
    cout << node->category << " (" << node->bookCount << " borrowed)" << endl;
    inorderRec(node->right);
}

void BST::destroyRec(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    destroyRec(node->left);
    destroyRec(node->right);
    delete node;
}

bool BST::searchRec(TreeNode* node, string cat) {
    if (node == nullptr) {
        return false;
    }

    if (cat == node->category) {
        return true;
    }

    if (cat < node->category) {
        return searchRec(node->left, cat);
    }

    return searchRec(node->right, cat);
}

void BST::incrementRec(TreeNode* node, string cat) {
    if (node == nullptr) {
        return;
    }

    if (cat == node->category) {
        node->bookCount++;
    } else if (cat < node->category) {
        incrementRec(node->left, cat);
    } else {
        incrementRec(node->right, cat);
    }
}

void BST::mostBorrowedRec(TreeNode* node, string& category, int& maxCount) {
    if (node == nullptr) {
        return;
    }

    mostBorrowedRec(node->left, category, maxCount);

    if (node->bookCount > maxCount) {
        maxCount = node->bookCount;
        category = node->category;
    }

    mostBorrowedRec(node->right, category, maxCount);
}

void BST::saveRec(TreeNode* node, ostream& out) {
    if (node == nullptr) {
        return;
    }

    saveRec(node->left, out);
    out << node->category << "\n";
    saveRec(node->right, out);
}

void BST::addCategory(string category) {
    root = insertRec(root, category);
}

void BST::deleteCategory(string category) {
    root = deleteRec(root, category);
}

void BST::displayTree() {
    if (root == nullptr) {
        cout << "No categories available.\n";
        return;
    }

    inorderRec(root);
}

bool BST::searchCategory(string category) {
    return searchRec(root, category);
}

void BST::incrementCount(string category) {
    if (!searchCategory(category)) {
        addCategory(category);
    }

    incrementRec(root, category);
}

string BST::getMostBorrowedCategory() {
    string category = "";
    int maxCount = -1;
    mostBorrowedRec(root, category, maxCount);
    return category;
}

void BST::saveCategories(ostream& out) {
    saveRec(root, out);
}
