#include "tree.h"
#include <iostream>
using namespace std;

BST::BST() {
    root = nullptr;
}

BST::~BST() {
    destroyRec(root);
}

void BST::clear() {
    destroyRec(root);
    root = nullptr;
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

void BST::setCountRec(TreeNode* node, string cat, int count) {
    if (node == nullptr) {
        return;
    }

    if (cat == node->category) {
        node->bookCount = count;
    } else if (cat < node->category) {
        setCountRec(node->left, cat, count);
    } else {
        setCountRec(node->right, cat, count);
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
    out << node->category << "," << node->bookCount << "\n";
    saveRec(node->right, out);
}

void BST::displayNumberedRec(TreeNode* node, int& number) {
    if (node == nullptr) {
        return;
    }

    displayNumberedRec(node->left, number);
    cout << number << ". " << node->category << endl;
    number++;
    displayNumberedRec(node->right, number);
}

string BST::getByIndexRec(TreeNode* node, int target, int& number) {
    if (node == nullptr) {
        return "";
    }


    string left = getByIndexRec(node->left, target, number);
    if (!left.empty()) {
        return left;
    }

    if (number == target) {
        return node->category;
    }
    number++;

    return getByIndexRec(node->right, target, number);
}

int BST::countRec(TreeNode* node) {
    if (node == nullptr) {
        return 0;
    }

    return 1 + countRec(node->left) + countRec(node->right);
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

void BST::displayCategoriesNumbered() {
    if (root == nullptr) {
        cout << "No categories available.\n";
        return;
    }

    int number = 1;
    displayNumberedRec(root, number);
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

void BST::setCount(string category, int count) {
    if (!searchCategory(category)) {
        addCategory(category);
    }

    setCountRec(root, category, count);
}

string BST::getMostBorrowedCategory() {
    string category = "";
    int maxCount = -1;
    mostBorrowedRec(root, category, maxCount);
    return category;
}

string BST::getCategoryByIndex(int index) {
    int number = 1;
    return getByIndexRec(root, index, number);
}

int BST::countCategories() {
    return countRec(root);
}

void BST::saveCategories(ostream& out) {
    saveRec(root, out);
}
