#pragma once
#include "models.h"
#include "bst.h"
#include "hashtable.h"
#include "graph.h"
#include "sorting.h"
#include <vector>
#include <queue>
#include <stack>
#include <string>

struct OrderCmp {
    bool operator()(const Order& a, const Order& b) const {
        return (int)a.priority < (int)b.priority;
    }
};

class AdminPanel {
private:
    RestaurantBST&   restaurantBST;
    OrderHashTable&  orderHash;
    DeliveryGraph&   graph;
    std::vector<Order>&           allOrders;
    std::vector<DeliveryPartner>& partners;
    std::priority_queue<Order, std::vector<Order>, OrderCmp>& orderQueue;
    std::stack<std::pair<std::string,std::string>>&           modStack;
    std::queue<Order>&            processingQueue;
    int& orderCounter;

    void orderManagementMenu();
    void routeMenu();
    void restaurantMenu();
    void hashMenu();
    void partnersMenu();
    void showDashboard();

    void viewAllOrders();
    void processNextOrder();
    void peekQueue();
    void updateOrderStatus();
    void undoModification();
    void viewStack();
    void trackAnyOrder();

    void printNodeList();
    void viewGraph();
    void runDijkstra();
    void runBFS();
    void runDFS();
    void compareAlgos();

    void viewRestaurants();
    void searchRestaurantByRating();
    void searchRestaurantByName();
    void addRestaurant();
    void viewBSTStructure();
    void bstStats();

public:
    AdminPanel(RestaurantBST& bst, OrderHashTable& ht, DeliveryGraph& g,
               std::vector<Order>& orders,
               std::vector<DeliveryPartner>& pts,
               std::priority_queue<Order, std::vector<Order>, OrderCmp>& oq,
               std::stack<std::pair<std::string,std::string>>& ms,
               std::queue<Order>& pq,
               int& counter);

    void run();
};

class CustomerPanel {
private:
    RestaurantBST&   restaurantBST;
    OrderHashTable&  orderHash;
    std::vector<Order>&           allOrders;
    std::vector<DeliveryPartner>& partners;
    std::priority_queue<Order, std::vector<Order>, OrderCmp>& orderQueue;
    std::queue<Order>&            processingQueue;
    std::stack<std::pair<std::string,std::string>>&           modStack;
    int& orderCounter;

    std::string customerName;
    std::string customerPhone;

    void browseAndOrder();
    void trackMyOrder();
    void printDeliveryMap(int step);   // ASCII grid map
    void printReceipt(const Order& o);
    void viewMyOrders();

public:
    CustomerPanel(RestaurantBST& bst, OrderHashTable& ht,
                  std::vector<Order>& orders,
                  std::vector<DeliveryPartner>& pts,
                  std::priority_queue<Order, std::vector<Order>, OrderCmp>& oq,
                  std::queue<Order>& pq,
                  std::stack<std::pair<std::string,std::string>>& ms,
                  int& counter);

    void run();
};
