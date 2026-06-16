# Food Delivery System

> Case Study No. 137 | Subrata Panda | Roll No: 150096725110
> Cohort: Larry Page | Academic Year: 2025–2029

---

## Table of Contents

- [2.1 Project Title](#21-project-title)
- [2.2 Problem Statement](#22-problem-statement)
- [2.3 Objectives](#23-objectives)
- [2.4 System Overview / Architecture](#24-system-overview--architecture)
- [2.5 Data Structures and Algorithms Used](#25-data-structures-and-algorithms-used)
- [2.6 Implementation Approach](#26-implementation-approach)
- [2.7 Time and Space Complexity Analysis](#27-time-and-space-complexity-analysis)
- [2.8 Execution Steps](#28-execution-steps)
- [2.9 Sample Inputs and Outputs](#29-sample-inputs-and-outputs)
- [2.10 Screenshots](#210-screenshots)
- [2.11 Results and Observations](#211-results-and-observations)
- [2.12 Conclusion](#212-conclusion)

---

## 2.1 Project Title

**Food Delivery System**

A console-based C++ application that manages restaurant orders, delivery partners, customer
requests, and delivery route optimisation using classical data structures and algorithms.
The system is divided into an **Admin Panel** for restaurant operators and a **Customer Portal**
for end users, both sharing a common in-memory data layer.

---

## 2.2 Problem Statement

Food delivery platforms require a system to manage restaurant orders, delivery partners,
customer requests, and route optimisation while ensuring timely deliveries. Many small
operations still rely on manual processes — phone-based order taking, ad-hoc rider assignment,
and no structured way to track an order from placement to delivery.

This leads to several recurring issues:

- **No prioritisation** — urgent or premium orders are treated the same as routine ones.
- **Inefficient search** — finding a specific order or restaurant record by hand is slow and error-prone.
- **No change tracking** — there is no record of who modified an order's status or when.
- **Suboptimal delivery routing** — riders are not assigned using shortest-path or fewest-hop logic.
- **Poor order traceability** — customers have no structured way to track where their order stands.

This project addresses these issues by modelling the food delivery domain directly on top of
well-known data structures and algorithms, so that every operational decision is backed by a
structure with known time complexity.

---

## 2.3 Objectives

- Prioritise urgent and premium deliveries using a **priority-queue-based** ordering system.
- Enable **O(1) average-time** order tracking through a custom hash table implementation.
- Maintain a searchable, self-balancing restaurant database using an **AVL-balanced BST**.
- Optimise delivery routes between restaurants, hubs, and customer zones using **graph algorithms**.
- Provide a modification history with **undo capability** using a stack-based audit trail.
- Separate concerns cleanly between an **Admin Panel** and a **Customer Portal**.
- Demonstrate sorting and searching algorithms (**merge sort, quick sort, binary search**)
  implemented from first principles.

---

## 2.4 System Overview / Architecture

The application follows a modular, layered architecture:

```
┌─────────────────────────────────────────────┐
│             PRESENTATION LAYER              │
│   Console menus — Admin Panel / Customer    │
├─────────────────────────────────────────────┤
│               LOGIC LAYER                  │
│    AdminPanel class | CustomerPanel class   │
├─────────────────────────────────────────────┤
│           DATA STRUCTURE LAYER             │
│  AVL-BST | Hash Table | Graph | PQ | Stack │
├─────────────────────────────────────────────┤
│             ALGORITHM LAYER                │
│  Merge Sort | Quick Sort | Dijkstra | BFS  │
├─────────────────────────────────────────────┤
│               PERSISTENCE                  │
│     In-memory only (process lifetime)      │
└─────────────────────────────────────────────┘
```

### Project File Structure

```
food-delivery-system/
├── main.cpp                  # Program entry point
├── Makefile                  # Build configuration
├── README.md                 # Project documentation
├── include/
│   ├── ui.h                  # Console colour macros and UI helpers
│   ├── models.h              # Order, Restaurant, DeliveryPartner structs
│   ├── bst.h                 # RestaurantBST (AVL tree) declaration
│   ├── graph.h               # DeliveryGraph declaration
│   ├── hashtable.h           # OrderHashTable declaration
│   ├── sorting.h             # Sorting and binary search declarations
│   └── panels.h              # AdminPanel and CustomerPanel declarations
└── src/
    ├── ui.cpp                # UI helper implementations
    ├── models.cpp            # Struct constructors and helpers
    ├── bst.cpp               # AVL-BST logic
    ├── graph.cpp             # Dijkstra, BFS, DFS
    ├── hashtable.cpp         # Hash function and chaining
    ├── sorting.cpp           # Merge sort, quick sort, binary search
    ├── admin.cpp             # Admin Panel screens
    ├── customer.cpp          # Customer Portal screens
    └── system.cpp            # Seed data and main menu routing
```

---

## 2.5 Data Structures and Algorithms Used

### Data Structures

| Structure | Where Used | Purpose |
|---|---|---|
| AVL-balanced BST | Restaurant database | Sorted by rating, guaranteed O(log n) height |
| Priority Queue (max-heap) | Order queue | Always serves highest-priority order next |
| Stack | Modification history | Tracks status changes for undo / audit |
| Queue (FIFO) | Order processing pipeline | Sequential intake of incoming orders |
| Hash Table (chaining) | Order lookup by ID | Near O(1) average lookup for tracking |
| Graph (adjacency list) | Delivery route network | Models restaurants, hubs, customer zones |

### Algorithms

| Algorithm | Purpose |
|---|---|
| Merge Sort | Sort orders by priority or customer name |
| Quick Sort | Sort orders by amount |
| Binary Search | Look up restaurant by exact name in sorted list |
| AVL Rotations | Rebalance BST after every insertion |
| Dijkstra's Algorithm | Shortest weighted path in delivery graph |
| Breadth-First Search | Fewest-hop path between two nodes |
| Depth-First Search | Exhaustive route exploration for comparison |

---

## 2.6 Implementation Approach

The system was built entirely in **C++** without any external libraries or database dependencies.
All data structures and algorithms were implemented from first principles.

**Key design decisions:**

1. **Shared in-memory state** — both panels operate on the same data layer passed by reference,
   ensuring order updates in the Admin Panel are immediately visible in the Customer Portal.

2. **Seed data on startup** — sample restaurants, delivery partners, orders, and a delivery graph
   are generated automatically so the system is demo-ready without manual input.

3. **Modular header/source pairs** — each data structure lives in its own `.h` / `.cpp` pair,
   keeping compilation units small and responsibilities clear.

4. **Console UI helpers** — a dedicated `ui.cpp` module handles coloured output, progress bars,
   bordered boxes, and receipt formatting, keeping business logic clean.

---

## 2.7 Time and Space Complexity Analysis

| Operation | Algorithm / Structure | Time Complexity | Space Complexity |
|---|---|---|---|
| Insert restaurant | AVL-BST with rebalancing | O(log n) | O(n) |
| Search restaurant by rating | AVL-BST search | O(log n) | O(1) |
| Search restaurant by name | Binary search (pre-sorted) | O(log n) | O(1) |
| Sort orders by priority / name | Merge Sort | O(n log n) | O(n) |
| Sort orders by amount | Quick Sort | O(n log n) avg, O(n²) worst | O(log n) |
| Insert / lookup order by ID | Hash Table with chaining | O(1) average | O(n) |
| Dequeue highest-priority order | Priority Queue (binary heap) | O(log n) | O(n) |
| Push / pop modification history | Stack | O(1) | O(n) |
| Shortest weighted delivery route | Dijkstra's Algorithm | O((V+E) log V) | O(V) |
| Fewest-hop delivery route | Breadth-First Search | O(V + E) | O(V) |
| Exhaustive route exploration | Depth-First Search | O(V + E) | O(V) |

> **Note:** The hash table flags a rehash recommendation once the load factor exceeds 0.75
> to maintain O(1) average performance.

---

## 2.8 Execution Steps

### Prerequisites

- g++ compiler (C++17 or later)
- `make` utility
- Linux / macOS terminal (or WSL on Windows)

### Build and Run

```bash
# 1. Clone the repository
git clone https://github.com/subratapanda24/food-delivery-system.git
cd food-delivery-system

# 2. Build the project
make

# 3. Run the application
./food_delivery
```

### Clean Build

```bash
make clean
make
```

### Navigation

```
Main Menu
├── [1] Admin Panel
│   ├── View Dashboard
│   ├── Process Next Order (dequeue)
│   ├── Assign Delivery Partner
│   ├── Update Order Status
│   ├── View Delivery Graph
│   └── Run Route Optimisation (Dijkstra / BFS / DFS)
└── [2] Customer Portal
    ├── Browse Restaurants (BST in-order)
    ├── Place Order
    ├── Track Order
    └── View Order History
```

---

## 2.9 Sample Inputs and Outputs

### Placing an Order (Customer Portal)

**Input:**
```
Select restaurant: 1  (Biryani Palace)
Select item (1-4, 0 to cancel): 1
Delivery priority — Choice (default 1): 4  (Urgent)
```

**Output:**
```
╔══════════════════════════════════════════════╗
║         FOOD DELIVERY ORDER RECEIPT          ║
╠══════════════════════════════════════════════╣
║ Order ID  : ORD-006                          ║
║ Customer  : Subrata Panda                    ║
║ Phone     : 7977xx                           ║
║ Placed at : 00:05:27                         ║
╠══════════════════════════════════════════════╣
║ Restaurant : Biryani Palace                  ║
║ Item       : Hyderabadi Biryani              ║
║ Priority   : URGENT                          ║
║ ETA        : 40 minutes                      ║
╠══════════════════════════════════════════════╣
║ TOTAL: Rs 340.00                             ║
║                                              ║
║    Thank you for ordering with us!           ║
╚══════════════════════════════════════════════╝
```

### BST Restaurant Search

**Input:**
```
Enter rating (e.g. 4.8): 4.8
```

**Output:**
```
[OK] Found: Biryani Palace
Biryani Palace   North Indian   * 4.8   342 orders   OPEN
BST height: 3   (AVL balanced, O(log n) guaranteed)
```

---

## 2.10 Screenshots

### Customer Menu and Order Receipt
![Customer Menu and Order Receipt](/Users/subratapanda/Desktop/Food Delivery/assets/Screenshot 2026-06-17 at 12.46.27 AM.png)

### Admin Dashboard — Live System Metrics
![Admin Dashboard](/Users/subratapanda/Desktop/Food Delivery/assets/Screenshot 2026-06-17 at 12.45.28 AM.png)

### Delivery Graph — Adjacency List
![Delivery Graph](/Users/subratapanda/Desktop/Food Delivery/assets/Screenshot 2026-06-17 at 12.08.39 AM.png)

Note: The terminal screenshot above is a capture of a scripted run of the program (placing a quick order) and shows the real receipt output produced by the binary.

---

## 2.11 Results and Observations

- The **priority queue** correctly dequeues orders in strict order: Urgent → High → Medium → Low,
  regardless of insertion sequence.
- The **hash table** consistently returns order records in O(1) average time, verified against
  linear-scan results for correctness.
- **AVL rotations** keep the restaurant BST balanced after repeated insertions, with observed tree
  height matching the theoretical O(log n) bound (height: 3 for 5 restaurants).
- **Dijkstra, BFS, and DFS** each produce a valid path between the same source and destination;
  Dijkstra and BFS agree on the optimal route when edge weights are uniform and diverge when
  they differ — as expected.
- The **modification-history stack** accurately records every status change in LIFO order,
  enabling a working undo of the most recent change.
- The customer **order tracking timeline** correctly reflects the order's current status as it
  is updated from the Admin Panel in real time.

### Limitations

- All data is held in memory; nothing persists once the program exits.
- No authentication layer — both panels are accessible to anyone running the binary.
- The delivery graph uses a small, hand-seeded node set rather than real geographic data.
- No payment processing is simulated; the receipt reflects order value only.

---

## 2.12 Conclusion

This project demonstrates that a functional, realistic food-delivery operations system can be
built entirely on top of classical data structures and algorithms — without a database or
external framework. Every core requirement of the domain maps directly onto a structure with
predictable, analysable time complexity:

| Requirement | Solution |
|---|---|
| Order prioritisation | Max-heap priority queue |
| Fast order lookup | Hash table with chaining |
| Searchable restaurant catalogue | AVL-balanced BST |
| Delivery route optimisation | Dijkstra / BFS / DFS on adjacency-list graph |
| Audit trail with undo | Stack-based modification history |

Implementing all structures and algorithms from first principles — rather than relying on
standard library containers — reinforced a practical understanding of how and why each
structure performs the way it does under real usage patterns.

**Future improvements:**
- Persist state to a file or SQLite database between runs.
- Add authentication for the Admin Panel.
- Replace the seeded delivery graph with real coordinate-based distances (OpenStreetMap / Haversine).
- Extend the Customer Portal with a payment simulation step.
- Add unit tests for each data structure module.

---

> **Repository:** https://github.com/subratapanda24/food-delivery-system.git
>
> **Declaration:** This project and report are the original work of Subrata Panda (150096725110). Case Study No. 137.