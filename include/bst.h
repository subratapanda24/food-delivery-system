#pragma once
#include "models.h"
#include <vector>

struct BSTNode {
    Restaurant data;
    BSTNode*   left;
    BSTNode*   right;
    int        height;
    explicit BSTNode(Restaurant r);
};

class RestaurantBST {
private:
    BSTNode* root;
    int      nodeCount;

    int      getHeight(BSTNode* node);
    int      getBalance(BSTNode* node);
    void     updateHeight(BSTNode* node);
    BSTNode* rotateRight(BSTNode* y);
    BSTNode* rotateLeft(BSTNode* x);
    BSTNode* balance(BSTNode* node);
    BSTNode* insert(BSTNode* node, const Restaurant& r);
    BSTNode* search(BSTNode* node, double rating);
    BSTNode* searchByName(BSTNode* node, const std::string& name);
    void     inOrder(BSTNode* node, std::vector<Restaurant>& result);
    void     printTree(BSTNode* node, const std::string& prefix,
                       bool isLeft, int depth);
    void     deleteTree(BSTNode* node);

public:
    RestaurantBST();
    ~RestaurantBST();

    void        insert(const Restaurant& r);
    Restaurant* search(double rating);
    Restaurant* searchByName(const std::string& name);

    std::vector<Restaurant> getSortedByRating();
    void                    printTree();

    int getHeight();
    int getCount();
};
