#include "../include/panels.h"
#include "../include/ui.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <limits>
#include <map>

using namespace std;

AdminPanel::AdminPanel(RestaurantBST& bst, OrderHashTable& ht, DeliveryGraph& g,
                       vector<Order>& orders, vector<DeliveryPartner>& pts,
                       priority_queue<Order,vector<Order>,OrderCmp>& oq,
                       stack<pair<string,string>>& ms,
                       queue<Order>& pq, int& counter)
    : restaurantBST(bst), orderHash(ht), graph(g),
      allOrders(orders), partners(pts),
      orderQueue(oq), modStack(ms), processingQueue(pq),
      orderCounter(counter) {}

void AdminPanel::run() {
    while (true) {
        clearScreen();
        cout << "\n";
        printBox("  ADMIN PANEL  --  Food Delivery System", CYAN);
        cout << "\n";
        cout << "  Time: " << getTimestamp()
             << "   Orders in system: " << allOrders.size()
             << "   Queue pending: " << orderQueue.size() << "\n\n";
        printLine(68);
        cout << "  [1]  Dashboard & Metrics\n";
        cout << "  [2]  Order Management       (Queue, Stack, Priority Heap)\n";
        cout << "  [3]  Route Optimization     (Dijkstra, BFS, DFS)\n";
        cout << "  [4]  Restaurant Database    (BST / AVL Tree)\n";
        cout << "  [5]  Hash Table Viewer      (Order Tracking)\n";
        cout << "  [6]  Delivery Partners\n";
        printLine(68);
        cout << "  [0]  Back to Main Menu\n\n";
        cout << "  Choice: ";
        int ch; cin >> ch;
        if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n'); continue; }
        cin.ignore();

        switch (ch) {
            case 1: showDashboard();       break;
            case 2: orderManagementMenu(); break;
            case 3: routeMenu();           break;
            case 4: restaurantMenu();      break;
            case 5: hashMenu();            break;
            case 6: partnersMenu();        break;
            case 0: return;
            default: printError("Enter 0-6.");
        }
    }
}

void AdminPanel::showDashboard() {
    clearScreen();
    cout << "\n";
    printBox("  DASHBOARD  --  Live System Metrics", CYAN);
    cout << "\n";

    int pending = 0, active = 0, delivered = 0;
    double revenue = 0;
    for (size_t i = 0; i < allOrders.size(); i++) {
        const Order& o = allOrders[i];
        if (o.status == OrderStatus::PENDING || o.status == OrderStatus::CONFIRMED) pending++;
        else if (o.status == OrderStatus::PREPARING || o.status == OrderStatus::ON_THE_WAY) active++;
        else if (o.status == OrderStatus::DELIVERED) { delivered++; revenue += o.amount; }
    }
    int availCount = 0;
    for (size_t i = 0; i < partners.size(); i++)
        if (partners[i].isAvailable) availCount++;

    printLine(68);
    cout << "  Pending orders    : " << pending   << "\n";
    cout << "  Active orders     : " << active    << "\n";
    cout << "  Delivered today   : " << delivered << "\n";
    cout << "  Revenue collected : Rs " << fixed << setprecision(0) << revenue << "\n";
    cout << "  Priority queue    : " << orderQueue.size() << " orders waiting\n";
    cout << "  Partners available: " << availCount << " / " << partners.size() << "\n";
    printLine(68);

    cout << "\n  PERFORMANCE\n";
    int total = (int)allOrders.size();
    printProgressBar("Completion rate",   total ? (double)delivered/total*100 : 0);
    printProgressBar("Partner available", partners.size() ? (double)availCount/partners.size()*100 : 0);
    printProgressBar("System load",       min(100.0,(double)(pending+active)*12));

    cout << "\n  TOP RESTAURANTS  (BST in-order traversal)\n";
    printLine(68);
    auto sorted = restaurantBST.getSortedByRating();
    for (int i = 0; i < min(3,(int)sorted.size()); i++) {
        cout << "  #" << i+1 << "  " << left << setw(20) << sorted[i].name
             << "  rating: " << fixed << setprecision(1) << sorted[i].rating
             << "   revenue: Rs" << fixed << setprecision(0) << sorted[i].revenue << "\n";
    }
    printLine(68);
    cout << "\n  [Enter] Back  ";
    cin.get();
}

void AdminPanel::orderManagementMenu() {
    while (true) {
        clearScreen();
        cout << "\n";
        printBox("  ORDER MANAGEMENT", CYAN);
        cout << "\n  Data structures in use: Priority Queue (max-heap), Stack, Hash Table\n\n";
        printLine(68);
        cout << "  [1]  View all orders      (sorted - Merge/Quick sort)\n";
        cout << "  [2]  Process next order   (Dequeue from priority heap)\n";
        cout << "  [3]  Peek priority queue  (View heap top without removing)\n";
        cout << "  [4]  Track any order      (Hash table O(1) lookup)\n";
        cout << "  [5]  Update order status\n";
        cout << "  [6]  Undo last change     (Stack pop)\n";
        cout << "  [7]  View change history  (Stack contents)\n";
        printLine(68);
        cout << "  [0]  Back\n\n";
        cout << "  Choice: ";
        int ch; cin >> ch;
        if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n'); continue; }
        cin.ignore();

        if      (ch == 0) break;
        else if (ch == 1) viewAllOrders();
        else if (ch == 2) processNextOrder();
        else if (ch == 3) peekQueue();
        else if (ch == 4) trackAnyOrder();
        else if (ch == 5) updateOrderStatus();
        else if (ch == 6) undoModification();
        else if (ch == 7) viewStack();
        else printError("Invalid choice.");
    }
}

void AdminPanel::viewAllOrders() {
    clearScreen();
    cout << "\n";
    printBox("  ALL ORDERS", CYAN);
    cout << "\n  Sort by:  [1] Priority   [2] Amount   [3] Customer Name\n  Choice: ";
    int s; cin >> s; cin.ignore();

    vector<Order> sorted = allOrders;
    string algo;
    if (s == 2) {
        quickSort(sorted, 0, (int)sorted.size()-1,
            [](const Order& a, const Order& b){ return a.amount > b.amount; });
        algo = "Quick Sort by amount desc   O(n log n) avg";
    } else if (s == 3) {
        mergeSort(sorted, 0, (int)sorted.size()-1,
            [](const Order& a, const Order& b){ return a.customerName < b.customerName; });
        algo = "Merge Sort by name          O(n log n)";
    } else {
        mergeSort(sorted, 0, (int)sorted.size()-1,
            [](const Order& a, const Order& b){ return (int)a.priority > (int)b.priority; });
        algo = "Merge Sort by priority      O(n log n)";
    }

    cout << "\n  Algorithm: " << algo << "\n\n";
    printLine(68);
    cout << "  " << left
         << setw(12) << "ORDER ID"
         << setw(18) << "CUSTOMER"
         << setw(16) << "RESTAURANT"
         << setw(9)  << "PRIORITY"
         << setw(13) << "STATUS"
         << "AMOUNT\n";
    printLine(68);
    for (size_t i = 0; i < sorted.size(); i++) printOrderRow(sorted[i], true);
    printLine(68);
    cout << "\n  " << sorted.size() << " orders total.\n";
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::processNextOrder() {
    clearScreen();
    cout << "\n";
    printBox("  PROCESS NEXT ORDER  --  Max-Heap Dequeue", CYAN);
    cout << "\n";

    if (orderQueue.empty()) {
        printWarning("Priority queue is empty.");
        cout << "\n  [Enter] Back  "; cin.get(); return;
    }

    Order top = orderQueue.top(); orderQueue.pop();
    printInfo("Dequeued from max-heap (highest priority):");
    cout << "\n";
    printOrderRow(top, true);
    cout << "\n";

    for (size_t i = 0; i < allOrders.size(); i++)
        if (allOrders[i].id == top.id) { allOrders[i].status = OrderStatus::PREPARING; break; }
    orderHash.update(top.id, OrderStatus::PREPARING);
    modStack.push({top.id, "PENDING -> PREPARING (dequeued)"});

    for (size_t i = 0; i < partners.size(); i++) {
        if (partners[i].isAvailable) {
            for (size_t j = 0; j < allOrders.size(); j++)
                if (allOrders[j].id == top.id) { allOrders[j].partnerName = partners[i].name; break; }
            printSuccess("Assigned to: " + partners[i].name + "  (" + partners[i].vehicle + ")");
            partners[i].isAvailable = false;
            break;
        }
    }
    printSuccess("Status set to PREPARING. Undo entry pushed to stack.");
    cout << "  Queue remaining: " << orderQueue.size() << " orders\n";
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::peekQueue() {
    clearScreen();
    cout << "\n";
    printBox("  PRIORITY QUEUE  --  Peek (no removal)", CYAN);
    cout << "\n";
    if (orderQueue.empty()) {
        printWarning("Queue is empty.");
    } else {
        const Order& top = orderQueue.top();
        printInfo("Top of max-heap  (highest priority):");
        cout << "\n";
        printOrderRow(top, true);
        cout << "\n  Queue size: " << orderQueue.size() << "\n";
    }
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::trackAnyOrder() {
    clearScreen();
    cout << "\n";
    printBox("  TRACK ORDER  --  Hash Table O(1) Lookup", CYAN);
    cout << "\n  Enter Order ID (e.g. ORD-001): ";
    string id; getline(cin, id);

    Order* o = orderHash.search(id);
    if (!o) {
        printError("Order " + id + " not found.");
        cout << "\n  [Enter] Back  "; cin.get(); return;
    }

    printSuccess("Found via hash table  (O(1) average)");
    cout << "\n";
    printLine(50);
    cout << "  Order ID   : " << o->id           << "\n";
    cout << "  Customer   : " << o->customerName << "\n";
    cout << "  Phone      : " << o->customerPhone<< "\n";
    cout << "  Restaurant : " << o->restaurantName<< "\n";
    cout << "  Item       : " << o->itemName     << "\n";
    cout << "  Amount     : Rs " << fixed << setprecision(0) << o->amount << "\n";
    cout << "  Priority   : " << priorityStr(o->priority) << "\n";
    cout << "  Status     : " << statusColor(o->status) << BOLD << statusStr(o->status) << RESET << "\n";
    cout << "  Partner    : " << (o->partnerName.empty() ? "Unassigned" : o->partnerName) << "\n";
    cout << "  ETA        : " << o->etaMinutes << " min\n";
    cout << "  Placed at  : " << o->timestamp  << "\n";
    printLine(50);
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::updateOrderStatus() {
    clearScreen();
    cout << "\n";
    printBox("  UPDATE ORDER STATUS", CYAN);
    cout << "\n  Enter Order ID: ";
    string id; getline(cin, id);

    Order* o = orderHash.search(id);
    if (!o) { printError("Order not found: " + id); cout << "\n  [Enter] Back  "; cin.get(); return; }

    cout << "\n  Current: " << statusColor(o->status) << BOLD << statusStr(o->status) << RESET << "\n\n";
    cout << "  [1] CONFIRMED\n  [2] PREPARING\n  [3] ON THE WAY\n  [4] DELIVERED\n  [5] CANCELLED\n\n";
    cout << "  New status: ";
    int ch; cin >> ch; cin.ignore();

    map<int,OrderStatus> smap;
    smap[1] = OrderStatus::CONFIRMED;
    smap[2] = OrderStatus::PREPARING;
    smap[3] = OrderStatus::ON_THE_WAY;
    smap[4] = OrderStatus::DELIVERED;
    smap[5] = OrderStatus::CANCELLED;
    if (!smap.count(ch)) { printError("Invalid."); cout << "\n  [Enter] Back  "; cin.get(); return; }

    string prev = statusStr(o->status);
    OrderStatus ns = smap[ch];

    map<OrderStatus,int> stepMap;
    stepMap[OrderStatus::CONFIRMED]  = 1;
    stepMap[OrderStatus::PREPARING]  = 2;
    stepMap[OrderStatus::ON_THE_WAY] = 3;
    stepMap[OrderStatus::DELIVERED]  = 4;
    if (stepMap.count(ns)) orderHash.updateTrackStep(id, stepMap[ns]);

    orderHash.update(id, ns);
    for (size_t i = 0; i < allOrders.size(); i++)
        if (allOrders[i].id == id) {
            allOrders[i].status = ns;
            if (stepMap.count(ns)) allOrders[i].trackStep = stepMap[ns];
            break;
        }
    modStack.push({id, prev + " -> " + statusStr(ns)});

    if (ns == OrderStatus::DELIVERED)
        for (size_t i = 0; i < partners.size(); i++)
            if (!partners[i].isAvailable) { partners[i].isAvailable = true; break; }

    printSuccess("Status updated:  " + prev + "  ->  " + statusStr(ns));
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::undoModification() {
    clearScreen();
    cout << "\n";
    printBox("  UNDO LAST CHANGE  --  Stack Pop", CYAN);
    cout << "\n";
    if (modStack.empty()) {
        printWarning("Stack is empty. Nothing to undo.");
    } else {
        string oid  = modStack.top().first;
        string desc = modStack.top().second;
        modStack.pop();
        printSuccess("Popped: [" + oid + "]  " + desc);
        cout << "  Stack entries remaining: " << modStack.size() << "\n";
        printInfo("Full state rollback requires persisting old values (this demo shows pop).");
    }
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::viewStack() {
    clearScreen();
    cout << "\n";
    printBox("  MODIFICATION HISTORY  --  Stack Contents", CYAN);
    cout << "\n  TOP\n";
    printLine(60);
    if (modStack.empty()) {
        cout << "  (stack is empty)\n";
    } else {
        stack<pair<string,string>> tmp = modStack;
        int i = 1;
        while (!tmp.empty()) {
            cout << "  [" << i++ << "]  " << BOLD << tmp.top().first << RESET
                 << "   " << DIM << tmp.top().second << RESET << "\n";
            tmp.pop();
        }
    }
    printLine(60);
    cout << "  BASE\n";
    cout << "\n  Total entries: " << modStack.size() << "   O(1) push/pop\n";
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::routeMenu() {
    while (true) {
        clearScreen();
        cout << "\n";
        printBox("  ROUTE OPTIMIZATION", CYAN);
        cout << "\n  Graph algorithms for delivery route planning\n\n";
        printLine(68);
        cout << "  [1]  View delivery graph\n";
        cout << "  [2]  Dijkstra  --  Shortest path (weighted)\n";
        cout << "  [3]  BFS       --  Fewest hops (unweighted)\n";
        cout << "  [4]  DFS       --  Depth-first exploration\n";
        cout << "  [5]  Compare all three algorithms\n";
        printLine(68);
        cout << "  [0]  Back\n\n";
        cout << "  Choice: ";
        int ch; cin >> ch;
        if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n'); continue; }
        cin.ignore();
        if      (ch == 0) break;
        else if (ch == 1) viewGraph();
        else if (ch == 2) runDijkstra();
        else if (ch == 3) runBFS();
        else if (ch == 4) runDFS();
        else if (ch == 5) compareAlgos();
        else printError("Invalid.");
    }
}

void AdminPanel::printNodeList() {
    auto& nodes = graph.getNodes();
    cout << "\n  Nodes in graph:\n";
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
        cout << "    " << left << setw(5) << it->first
             << " -- " << it->second.name
             << "  (" << it->second.type << ")\n";
    cout << "\n";
}

void AdminPanel::viewGraph() {
    clearScreen();
    cout << "\n";
    printBox("  DELIVERY GRAPH  --  Adjacency List", CYAN);
    graph.printGraph();
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::runDijkstra() {
    clearScreen();
    cout << "\n";
    printBox("  DIJKSTRA  --  O((V+E) log V)", CYAN);
    printNodeList();
    cout << "  Source node ID : "; string src; getline(cin, src);
    cout << "  Destination ID : "; string dst; getline(cin, dst);

    auto result = graph.dijkstra(src);
    auto& dist  = result.first;
    auto& prev  = result.second;

    if (!dist.count(dst) || dist[dst] == numeric_limits<double>::infinity()) {
        printError("No path from " + src + " to " + dst);
        cout << "\n  [Enter] Back  "; cin.get(); return;
    }
    vector<string> path = graph.getPath(prev, src, dst);
    cout << "\n  Shortest path:\n  ";
    graph.printPath(path, dist[dst]);

    cout << "\n  Full distance table:\n";
    printLine(50);
    for (auto it = dist.begin(); it != dist.end(); ++it) {
        cout << "  " << left << setw(6) << it->first << "  ";
        if (it->second == numeric_limits<double>::infinity())
            cout << "unreachable\n";
        else
            cout << fixed << setprecision(2) << it->second << " km\n";
    }
    printLine(50);
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::runBFS() {
    clearScreen();
    cout << "\n";
    printBox("  BFS  --  Breadth-First Search  O(V+E)", CYAN);
    printNodeList();
    cout << "  Source node ID : "; string src; getline(cin, src);
    cout << "  Destination ID : "; string dst; getline(cin, dst);
    vector<string> path = graph.bfs(src, dst);
    cout << "\n  BFS path (fewest hops):\n  ";
    graph.printPath(path, graph.pathDistance(path));
    printInfo("BFS guarantees minimum hops, not minimum distance.");
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::runDFS() {
    clearScreen();
    cout << "\n";
    printBox("  DFS  --  Depth-First Search  O(V+E)", CYAN);
    printNodeList();
    cout << "  Source node ID : "; string src; getline(cin, src);
    cout << "  Destination ID : "; string dst; getline(cin, dst);
    vector<string> path = graph.dfs(src, dst);
    cout << "\n  DFS path (first found):\n  ";
    graph.printPath(path, graph.pathDistance(path));
    printInfo("DFS does not guarantee shortest path.");
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::compareAlgos() {
    clearScreen();
    cout << "\n";
    printBox("  ALGORITHM COMPARISON  --  R1 to C2", CYAN);
    string src = "R1", dst = "C2";

    auto result  = graph.dijkstra(src);
    auto& prev   = result.second;
    auto& dist   = result.first;
    (void)dist;
    vector<string> dijPath = graph.getPath(prev, src, dst);
    vector<string> bfsPath = graph.bfs(src, dst);
    vector<string> dfsPath = graph.dfs(src, dst);

    cout << "\n  Route: Biryani Palace  -->  Customer Zone B\n\n";
    printLine(68);

    cout << "  Dijkstra  (shortest weighted distance):\n  ";
    graph.printPath(dijPath, graph.pathDistance(dijPath));
    cout << "  Hops: " << (dijPath.size() > 1 ? dijPath.size()-1 : 0)
         << "   Complexity: O((V+E) log V)\n\n";

    cout << "  BFS  (fewest hops):\n  ";
    graph.printPath(bfsPath, graph.pathDistance(bfsPath));
    cout << "  Hops: " << (bfsPath.size() > 1 ? bfsPath.size()-1 : 0)
         << "   Complexity: O(V+E)\n\n";

    cout << "  DFS  (depth-first, first path found):\n  ";
    graph.printPath(dfsPath, graph.pathDistance(dfsPath));
    cout << "  Hops: " << (dfsPath.size() > 1 ? dfsPath.size()-1 : 0)
         << "   Complexity: O(V+E)\n\n";

    printLine(68);
    cout << "  Summary:\n";
    cout << "  Dijkstra  --  optimal for weighted graphs, delivery distance\n";
    cout << "  BFS       --  optimal for unweighted/hop-count routing\n";
    cout << "  DFS       --  useful for exhaustive route exploration\n";
    printLine(68);
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::restaurantMenu() {
    while (true) {
        clearScreen();
        cout << "\n";
        printBox("  RESTAURANT DATABASE  --  AVL / BST", CYAN);
        cout << "\n";
        printLine(68);
        cout << "  [1]  List all          (in-order traversal)\n";
        cout << "  [2]  Search by rating  (BST search  O(log n))\n";
        cout << "  [3]  Search by name    (binary search O(log n))\n";
        cout << "  [4]  Add restaurant    (BST insert with AVL rebalance)\n";
        cout << "  [5]  View tree shape\n";
        cout << "  [6]  BST statistics\n";
        printLine(68);
        cout << "  [0]  Back\n\n";
        cout << "  Choice: ";
        int ch; cin >> ch;
        if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n'); continue; }
        cin.ignore();
        if      (ch == 0) break;
        else if (ch == 1) viewRestaurants();
        else if (ch == 2) searchRestaurantByRating();
        else if (ch == 3) searchRestaurantByName();
        else if (ch == 4) addRestaurant();
        else if (ch == 5) viewBSTStructure();
        else if (ch == 6) bstStats();
        else printError("Invalid.");
    }
}

void AdminPanel::viewRestaurants() {
    clearScreen();
    cout << "\n";
    printBox("  RESTAURANTS  --  Sorted by Rating", CYAN);
    cout << "\n";
    printLine(68);
    cout << "  " << left << setw(20) << "NAME" << setw(16) << "CUISINE"
         << setw(8) << "RATING" << setw(12) << "ORDERS" << "STATUS\n";
    printLine(68);
    auto sorted = restaurantBST.getSortedByRating();
    for (size_t i = 0; i < sorted.size(); i++) printRestRow(sorted[i]);
    printLine(68);
    printInfo("Listed via BST in-order traversal  O(n)");
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::searchRestaurantByRating() {
    clearScreen();
    cout << "\n";
    printBox("  SEARCH BY RATING  --  BST O(log n)", CYAN);
    cout << "\n  Enter rating (e.g. 4.8): ";
    double r; cin >> r; cin.ignore();
    if (r < 1 || r > 5) { printError("Rating must be 1.0-5.0."); cout << "\n  [Enter] Back  "; cin.get(); return; }
    Restaurant* res = restaurantBST.search(r);
    if (!res) {
        printWarning("No restaurant with rating exactly " + to_string(r));
    } else {
        printSuccess("Found: " + res->name);
        cout << "\n";
        printRestRow(*res);
        cout << "\n  BST height: " << restaurantBST.getHeight()
             << "   (AVL balanced, O(log n) guaranteed)\n";
    }
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::searchRestaurantByName() {
    clearScreen();
    cout << "\n";
    printBox("  SEARCH BY NAME  --  Binary Search O(log n)", CYAN);
    cout << "\n  Enter restaurant name: ";
    string name; getline(cin, name);
    if (name.empty()) { printError("Name cannot be empty."); cout << "\n  [Enter] Back  "; cin.get(); return; }

    auto sorted = restaurantBST.getSortedByRating();
    vector<Restaurant> byName = sorted;
    sort(byName.begin(), byName.end(),
         [](const Restaurant& a, const Restaurant& b){ return a.name < b.name; });
    int idx = binarySearchRestaurant(byName, name);
    if (idx == -1) {
        printWarning("'" + name + "' not found.");
    } else {
        printSuccess("Found at sorted index " + to_string(idx) + "  (binary search)");
        cout << "\n";
        printRestRow(byName[idx]);
        cout << "\n  Max comparisons: ~" << (int)ceil(log2(byName.size()+1)) << "\n";
    }
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::addRestaurant() {
    clearScreen();
    cout << "\n";
    printBox("  ADD RESTAURANT  --  BST Insert O(log n)", CYAN);
    cout << "\n";
    Restaurant r;
    cout << "  Name      : "; getline(cin, r.name);
    if (r.name.empty()) { printError("Name required."); cout << "\n  [Enter] Back  "; cin.get(); return; }
    cout << "  Cuisine   : "; getline(cin, r.cuisine);
    cout << "  Rating    : "; cin >> r.rating; cin.ignore();
    if (r.rating < 1 || r.rating > 5) { printError("Rating 1-5 only."); cout << "\n  [Enter] Back  "; cin.get(); return; }
    cout << "  Prep time : "; cin >> r.avgPrepTime; cin.ignore();
    cout << "  Address   : "; getline(cin, r.address);
    r.totalOrders = 0; r.revenue = 0; r.isOpen = true;

    restaurantBST.insert(r);
    printSuccess("'" + r.name + "' inserted into AVL-BST.");
    cout << "  New tree height: " << restaurantBST.getHeight()
         << "   Total nodes: " << restaurantBST.getCount() << "\n";
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::viewBSTStructure() {
    clearScreen();
    cout << "\n";
    printBox("  BST TREE SHAPE  --  AVL balanced", CYAN);
    cout << "\n  (ordered by rating, each node AVL-balanced)\n";
    restaurantBST.printTree();
    cout << "\n  Height: " << restaurantBST.getHeight()
         << "   Nodes: " << restaurantBST.getCount() << "\n";
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::bstStats() {
    clearScreen();
    cout << "\n";
    printBox("  BST STATISTICS", CYAN);
    cout << "\n";
    int n = restaurantBST.getCount();
    int h = restaurantBST.getHeight();
    int ideal = (int)ceil(log2(n+1));
    printLine(50);
    cout << "  Total nodes      : " << n     << "\n";
    cout << "  Current height   : " << h     << "\n";
    cout << "  Ideal min height : " << ideal << "  (perfect binary tree)\n";
    cout << "  Search cost      : O(log n)  ~" << ideal << " comparisons\n";
    cout << "  Insert cost      : O(log n)  with AVL rebalance\n";
    printLine(50);
    printProgressBar("AVL balance quality",
        max(0.0, 100.0 - (h - ideal)*20.0));
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::hashMenu() {
    clearScreen();
    cout << "\n";
    printBox("  HASH TABLE  --  Order Tracking O(1)", CYAN);
    cout << "\n  Hash function: h(key) = sum(char * 31^i) mod 16\n";
    cout << "  Collision resolution: chaining (linked buckets)\n\n";
    orderHash.printTable();
    cout << "\n";
    printProgressBar("Load factor", (double)orderHash.getCount()/16*100);
    printInfo("Rehash recommended when load factor exceeds 0.75");
    cout << "\n  [Enter] Back  "; cin.get();
}

void AdminPanel::partnersMenu() {
    clearScreen();
    cout << "\n";
    printBox("  DELIVERY PARTNERS", CYAN);
    cout << "\n";
    printLine(68);
    cout << "  " << left << setw(6) << "ID" << setw(18) << "NAME"
         << setw(10) << "VEHICLE" << setw(8) << "RATING"
         << setw(8)  << "TRIPS"   << "STATUS\n";
    printLine(68);

    int avail = 0;
    for (size_t i = 0; i < partners.size(); i++) {
        const DeliveryPartner& p = partners[i];
        if (p.isAvailable) avail++;
        string sc = p.isAvailable ? GREEN : YELLOW;
        cout << "  " << left << setw(6) << p.id
             << setw(18) << p.name
             << setw(10) << p.vehicle
             << setw(8)  << fixed << setprecision(1) << p.rating
             << setw(8)  << p.totalDeliveries
             << sc << (p.isAvailable ? "Available" : "On delivery") << RESET << "\n";

        printProgressBar(p.name,
            min(100.0, p.rating / 5.0 * 100), 20,
            p.isAvailable ? GREEN : YELLOW);
    }
    printLine(68);
    cout << "\n  Available: " << avail << " / " << partners.size() << "\n";
    cout << "\n  [Enter] Back  "; cin.get();
}
