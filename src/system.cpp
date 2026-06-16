#include "../include/panels.h"
#include "../include/ui.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <stack>
#include <limits>

using namespace std;

class FoodDeliverySystem {
private:
    RestaurantBST    restaurantBST;
    OrderHashTable   orderHash;
    DeliveryGraph    graph;

    vector<Order>           allOrders;
    vector<DeliveryPartner> partners;

    priority_queue<Order, vector<Order>, OrderCmp> orderQueue;
    stack<pair<string,string>>                     modStack;
    queue<Order>                                   processingQueue;

    int orderCounter;

    void seedRestaurants() {
        vector<Restaurant> rests = {
            {"Biryani Palace", "North Indian", 4.8, 342, 48200, 25, true,  "12 MG Road, Zone A"},
            {"Pizza Hub",      "Italian",      4.5, 278, 38900, 20, true,  "45 Park Street, Zone B"},
            {"Burger Barn",    "Fast Food",    4.3, 198, 22100, 15, true,  "7 Main St, Zone C"},
            {"Wok Express",    "Chinese",      4.6, 221, 31400, 18, true,  "22 Hill View, Zone B"},
            {"Dosa Corner",    "South Indian", 4.7, 310, 18700, 12, true,  "5 Temple Road, Zone A"},
            {"Chai Stop",      "Beverages",    4.4, 156,  9800,  8, false, "18 Lake Side, Zone D"},
        };
        for (size_t i = 0; i < rests.size(); i++)
            restaurantBST.insert(rests[i]);
    }

    void seedPartners() {
        partners = {
            {"P001", "Ravi Kumar",   "Bike",  4.9, 1240, true,  "Zone A", 18400},
            {"P002", "Mohan Singh",  "Cycle", 4.7,  980, true,  "Zone B", 14200},
            {"P003", "Deepak Rao",   "Bike",  4.6,  740, false, "Zone C", 10800},
            {"P004", "Suresh Patel", "Bike",  4.8, 1100, true,  "Zone A", 16600},
            {"P005", "Kiran Das",    "Cycle", 4.5,  620, true,  "Zone D",  8900},
        };
    }

    void seedOrders() {
        vector<Order> sample = {
            {"ORD-001","Arjun Mehta", "+91-9876","Biryani Palace","Hyderabadi Biryani",340,Priority::HIGH,   8},
            {"ORD-002","Priya Shah",  "+91-9812","Pizza Hub",     "Margherita XL",     280,Priority::MEDIUM,15},
            {"ORD-003","Rahul Verma", "+91-9845","Burger Barn",   "Zinger Combo",      190,Priority::LOW,   20},
            {"ORD-004","Sneha Joshi", "+91-9801","Dosa Corner",   "Masala Dosa Set",   120,Priority::URGENT, 5},
            {"ORD-005","Vikram Nair", "+91-9834","Wok Express",   "Hakka Noodles",     210,Priority::MEDIUM,18},
        };
        sample[0].trackStep = 2;
        sample[1].trackStep = 3;
        sample[2].trackStep = 0;
        sample[3].trackStep = 4;
        sample[4].trackStep = 1;

        for (size_t i = 0; i < sample.size(); i++) {
            allOrders.push_back(sample[i]);
            orderHash.insert(sample[i]);
            orderQueue.push(sample[i]);
            processingQueue.push(sample[i]);
        }
        orderCounter = 6;
    }

    void seedGraph() {
        graph.addNode("R1", "Biryani Palace",  "restaurant");
        graph.addNode("R2", "Pizza Hub",       "restaurant");
        graph.addNode("R3", "Wok Express",     "restaurant");
        graph.addNode("H1", "Central Hub",     "hub");
        graph.addNode("H2", "North Hub",       "hub");
        graph.addNode("C1", "Customer Zone A", "customer");
        graph.addNode("C2", "Customer Zone B", "customer");
        graph.addNode("C3", "Customer Zone C", "customer");

        graph.addEdge("R1","H1", 3.2);
        graph.addEdge("R2","H2", 2.8);
        graph.addEdge("R3","H1", 2.1);
        graph.addEdge("H1","H2", 1.5);
        graph.addEdge("H1","C1", 4.0);
        graph.addEdge("H1","C3", 5.2);
        graph.addEdge("H2","C2", 3.8);
        graph.addEdge("H2","C1", 4.5);
        graph.addEdge("R1","C1", 6.0);
        graph.addEdge("R2","C2", 5.5);
    }

    void printHeader() {
        clearScreen();
        cout << "\n";
        cout << "  +------------------------------------------------------------------+\n";
        cout << "  |                       Food Delivery System                       |\n";
        cout << "  +------------------------------------------------------------------+\n";
        cout << "\n  Time: " << getTimestamp()
             << "   Orders: " << allOrders.size()
             << "   Queue: " << orderQueue.size() << " pending\n";
    }

public:
    FoodDeliverySystem() : orderCounter(6) {
        seedRestaurants();
        seedPartners();
        seedOrders();
        seedGraph();
    }

    void run() {
        while (true) {
            printHeader();
            cout << "\n";
            cout << "  ----------------------------------------------------------------\n";
            cout << "  ADMIN                             CUSTOMER\n";
            cout << "  ----------------------------------------------------------------\n";
            cout << "  [1]  Admin Panel                  [2]  Customer Portal\n";
            cout << "  ----------------------------------------------------------------\n";
            cout << "  [0]  Exit\n\n";
            cout << "  Choice: ";

            int ch; cin >> ch;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore();

            if (ch == 0) {
                clearScreen();
                cout << "\n\n  Thank you for using Food Delivery System.\n\n";
                return;
            } else if (ch == 1) {
                AdminPanel admin(restaurantBST, orderHash, graph,
                                 allOrders, partners,
                                 orderQueue, modStack, processingQueue,
                                 orderCounter);
                admin.run();
            } else if (ch == 2) {
                CustomerPanel customer(restaurantBST, orderHash,
                                       allOrders, partners,
                                       orderQueue, processingQueue,
                                       modStack, orderCounter);
                customer.run();
            } else {
                printError("Enter 0, 1, or 2.");
            }
        }
    }
};

void runSystem() {
    FoodDeliverySystem sys;
    sys.run();
}
