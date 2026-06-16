#include "../include/bst.h"
#include "../include/ui.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

// AVL Tree
BSTNode::BSTNode(Restaurant r)
    : data(r), left(nullptr), right(nullptr), height(1) {}

int RestaurantBST::getHeight(BSTNode* node) {
    return node ? node->height : 0;
}

int RestaurantBST::getBalance(BSTNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

void RestaurantBST::updateHeight(BSTNode* node) {
    if (node)
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

BSTNode* RestaurantBST::rotateRight(BSTNode* y) {
    BSTNode* x  = y->left;
    BSTNode* T2 = x->right;
    x->right = y;
    y->left  = T2;
    updateHeight(y);
    updateHeight(x);
    return x;
}

BSTNode* RestaurantBST::rotateLeft(BSTNode* x) {
    BSTNode* y  = x->right;
    BSTNode* T2 = y->left;
    y->left  = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);
    return y;
}

BSTNode* RestaurantBST::balance(BSTNode* node) {
    updateHeight(node);
    int bf = getBalance(node);
    if (bf > 1  && getBalance(node->left)  >= 0) return rotateRight(node);
    if (bf > 1  && getBalance(node->left)  <  0) { node->left  = rotateLeft(node->left);  return rotateRight(node); }
    if (bf < -1 && getBalance(node->right) <= 0) return rotateLeft(node);
    if (bf < -1 && getBalance(node->right) >  0) { node->right = rotateRight(node->right); return rotateLeft(node); }
    return node;
}

BSTNode* RestaurantBST::insert(BSTNode* node, const Restaurant& r) {
    if (!node) { nodeCount++; return new BSTNode(r); }
    if (r.rating < node->data.rating)
        node->left  = insert(node->left,  r);
    else if (r.rating > node->data.rating)
        node->right = insert(node->right, r);
    else {
        if (r.name < node->data.name)
            node->left  = insert(node->left,  r);
        else
            node->right = insert(node->right, r);
    }
    return balance(node);
}

BSTNode* RestaurantBST::search(BSTNode* node, double rating) {
    if (!node) return nullptr;
    if (fabs(node->data.rating - rating) < 0.001) return node;
    if (rating < node->data.rating) return search(node->left,  rating);
    return search(node->right, rating);
}

BSTNode* RestaurantBST::searchByName(BSTNode* node, const string& name) {
    if (!node) return nullptr;
    if (node->data.name == name) return node;
    BSTNode* left = searchByName(node->left, name);
    if (left) return left;
    return searchByName(node->right, name);
}

void RestaurantBST::inOrder(BSTNode* node, vector<Restaurant>& result) {
    if (!node) return;
    inOrder(node->left, result);
    result.push_back(node->data);
    inOrder(node->right, result);
}

void RestaurantBST::printTree(BSTNode* node, const string& prefix,
                              bool isLeft, int depth) {
    if (!node || depth > 4) return;
    cout << "  " << prefix;
    cout << (isLeft ? "+-- " : "\\-- ");
    cout << BOLD << node->data.name << RESET;
    cout << DIM << " (*" << fixed << setprecision(1) << node->data.rating << ")" << RESET << "\n";
    string newPrefix = prefix + (isLeft ? "|   " : "    ");
    printTree(node->left,  newPrefix, true,  depth + 1);
    printTree(node->right, newPrefix, false, depth + 1);
}

void RestaurantBST::deleteTree(BSTNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

RestaurantBST::RestaurantBST() : root(nullptr), nodeCount(0) {}

RestaurantBST::~RestaurantBST() { deleteTree(root); }

void RestaurantBST::insert(const Restaurant& r) {
    root = insert(root, r);
}

Restaurant* RestaurantBST::search(double rating) {
    BSTNode* node = search(root, rating);
    return node ? &node->data : nullptr;
}

Restaurant* RestaurantBST::searchByName(const string& name) {
    BSTNode* node = searchByName(root, name);
    return node ? &node->data : nullptr;
}

vector<Restaurant> RestaurantBST::getSortedByRating() {
    vector<Restaurant> result;
    inOrder(root, result);
    reverse(result.begin(), result.end());
    return result;
}

void RestaurantBST::printTree() {
    if (!root) { printError("BST is empty."); return; }
    cout << "\n  " << BOLD << root->data.name << RESET;
    cout << DIM << " (*" << fixed << setprecision(1)
         << root->data.rating << ") [ROOT]\n" << RESET;
    printTree(root->left,  "  ", true,  1);
    printTree(root->right, "  ", false, 1);
}

int RestaurantBST::getHeight() { return getHeight(root); }
int RestaurantBST::getCount()  { return nodeCount; }
