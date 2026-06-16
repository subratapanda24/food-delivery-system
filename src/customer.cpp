#include "../include/panels.h"
#include "../include/ui.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>
#include <ctime>

using namespace std;

static const MenuItem MENU_BIRYANI[] = {
    {"Hyderabadi Biryani",  "Slow-cooked basmati with saffron & tender mutton",   340, "Main"},
    {"Veg Biryani",         "Fragrant rice with seasonal vegetables & whole spices",220, "Main"},
    {"Chicken Tikka",       "Chargrilled marinated chicken, mint chutney",         280, "Starter"},
    {"Raita",               "Chilled yogurt with cucumber & roasted cumin",         60, "Side"},
};
static const MenuItem MENU_PIZZA[] = {
    {"Margherita XL",       "San Marzano tomato, buffalo mozzarella, fresh basil", 280, "Pizza"},
    {"Pepperoni",           "Double pepperoni, aged cheddar, oregano",             320, "Pizza"},
    {"BBQ Chicken",         "Smoked chicken, caramelised onion, BBQ drizzle",      340, "Pizza"},
    {"Garlic Bread",        "Toasted ciabatta, garlic butter, herbs",               90, "Side"},
};
static const MenuItem MENU_BURGER[] = {
    {"Zinger Combo",        "Crispy chicken, lettuce, mayo, waffle fries",         190, "Combo"},
    {"Double Smash",        "Two smashed beef patties, American cheese, pickles",  220, "Burger"},
    {"Veg Supreme",         "Chickpea patty, avocado, tomato relish",              160, "Burger"},
    {"Chocolate Shake",     "Thick Belgian chocolate milkshake",                    90, "Drink"},
};
static const MenuItem MENU_WOK[] = {
    {"Hakka Noodles",       "Wok-tossed egg noodles, vegetables, soy sauce",      210, "Noodles"},
    {"Manchurian Gravy",    "Crispy veg dumplings in tangy Manchurian sauce",      190, "Gravy"},
    {"Fried Rice",          "Jasmine rice, egg, spring onion, dark soy",           180, "Rice"},
    {"Hot & Sour Soup",     "Classic Chinese broth with mushroom & tofu",          120, "Soup"},
};
static const MenuItem MENU_DOSA[] = {
    {"Masala Dosa Set",     "Crispy dosa, spiced potato filling, coconut chutney", 120, "Dosa"},
    {"Uttapam",             "Thick rice pancake with onion, tomato & coriander",   110, "Dosa"},
    {"Idli Sambar",         "Steamed rice cakes with lentil broth & chutneys",      90, "Tiffin"},
    {"Filter Coffee",       "South Indian drip coffee, rich & frothy",              50, "Drink"},
};
static const MenuItem MENU_CHAI[] = {
    {"Masala Chai",         "Ginger, cardamom, clove brewed tea with full-cream",   40, "Hot"},
    {"Cold Coffee",         "Blended coffee with vanilla ice cream",                 70, "Cold"},
    {"Samosa (2 pcs)",      "Crispy pastry stuffed with spiced potato & peas",      60, "Snack"},
};

struct RestaurantMenu {
    const string     name;
    const MenuItem*  items;
    int              count;
};

static const RestaurantMenu ALL_MENUS[] = {
    {"Biryani Palace", MENU_BIRYANI, 4},
    {"Pizza Hub",      MENU_PIZZA,   4},
    {"Burger Barn",    MENU_BURGER,  4},
    {"Wok Express",    MENU_WOK,     4},
    {"Dosa Corner",    MENU_DOSA,    4},
    {"Chai Stop",      MENU_CHAI,    3},
};
static const int NUM_MENUS = 6;

CustomerPanel::CustomerPanel(RestaurantBST& bst, OrderHashTable& ht,
                             vector<Order>& orders,
                             vector<DeliveryPartner>& pts,
                             priority_queue<Order,vector<Order>,OrderCmp>& oq,
                             queue<Order>& pq,
                             stack<pair<string,string>>& ms,
                             int& counter)
    : restaurantBST(bst), orderHash(ht),
      allOrders(orders), partners(pts),
      orderQueue(oq), processingQueue(pq),
      modStack(ms), orderCounter(counter) {}

static string generateId(int& counter) {
    string num = to_string(counter);
    string padded = string(3 - min((int)num.size(), 3), '0') + num;
    counter++;
    return "ORD-" + padded;
}

void CustomerPanel::run() {
    clearScreen();
    cout << "\n";
    printBox("  CUSTOMER PORTAL  --  Food Delivery System", CYAN);
    cout << "\n";
    cout << "  Your name  : "; getline(cin, customerName);
    if (customerName.empty()) customerName = "Guest";
    cout << "  Your phone : "; getline(cin, customerPhone);
    if (customerPhone.empty()) customerPhone = "N/A";

    while (true) {
        clearScreen();
        cout << "\n";
        printBox("  CUSTOMER PORTAL  --  Food Delivery System", CYAN);
        cout << "\n  Hello, " << customerName << "\n\n";
        printLine(68);
        cout << "  [1]  Browse menu & place order\n";
        cout << "  [2]  Track my order\n";
        cout << "  [3]  My order history\n";
        printLine(68);
        cout << "  [0]  Back to Main Menu\n\n";
        cout << "  Choice: ";
        int ch; cin >> ch;
        if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n'); continue; }
        cin.ignore();

        if      (ch == 0) break;
        else if (ch == 1) browseAndOrder();
        else if (ch == 2) trackMyOrder();
        else if (ch == 3) viewMyOrders();
        else printError("Enter 0-3.");
    }
}

void CustomerPanel::browseAndOrder() {
    clearScreen();
    cout << "\n";
    printBox("  RESTAURANTS", CYAN);

    auto rests = restaurantBST.getSortedByRating();
    cout << "\n";
    printLine(68);
    for (int i = 0; i < (int)rests.size(); i++) {
        const Restaurant& r = rests[i];
        string status = r.isOpen ? (string(GREEN) + "Open" + RESET)
                                 : (string(BRED)  + "Closed" + RESET);
        cout << "  [" << i+1 << "]  " << left << setw(20) << r.name
             << "  " << setw(14) << r.cuisine
             << "  Rating: " << fixed << setprecision(1) << r.rating
             << "   " << status << "\n";
    }
    printLine(68);
    cout << "\n  Select restaurant (1-" << rests.size() << ", 0 to cancel): ";
    int ri; cin >> ri; cin.ignore();
    if (ri < 1 || ri > (int)rests.size()) return;

    const Restaurant& chosen = rests[ri-1];
    if (!chosen.isOpen) { printWarning("This restaurant is currently closed."); cout << "\n  [Enter] Back  "; cin.get(); return; }

    const RestaurantMenu* menu = nullptr;
    for (int m = 0; m < NUM_MENUS; m++)
        if (ALL_MENUS[m].name == chosen.name) { menu = &ALL_MENUS[m]; break; }

    clearScreen();
    cout << "\n";
    printBox("  MENU  --  " + chosen.name, CYAN);
    cout << "\n  " << chosen.cuisine << "  |  Rating: "
         << fixed << setprecision(1) << chosen.rating
         << "  |  Prep time: ~" << chosen.avgPrepTime << " min\n";
    cout << "  " << chosen.address << "\n\n";

    printLine(68);
    cout << "  " << left << setw(4) << "#"
         << setw(26) << "ITEM"
         << setw(10) << "CATEGORY"
         << "PRICE\n";
    printLine(68);

    if (menu) {
        for (int i = 0; i < menu->count; i++) {
            const MenuItem& item = menu->items[i];
            cout << "  [" << i+1 << "] " << left << setw(24) << item.name
                 << setw(12) << item.category
                 << "Rs " << fixed << setprecision(0) << item.price << "\n";
            cout << "       " << DIM << item.description << RESET << "\n";
        }
    }
    printLine(68);

    cout << "\n  Select item (1-" << (menu ? menu->count : 0) << ", 0 to cancel): ";
    int ii; cin >> ii; cin.ignore();
    if (!menu || ii < 1 || ii > menu->count) return;

    const MenuItem& selectedItem = menu->items[ii-1];

    cout << "\n  Delivery priority:\n";
    cout << "    [1] Standard  (Low)      - normal queue\n";
    cout << "    [2] Priority  (Medium)   - faster\n";
    cout << "    [3] Express   (High)     - priority lane\n";
    cout << "    [4] Urgent    (Urgent)   - next in queue\n";
    cout << "  Choice (default 1): ";
    int pi; cin >> pi; cin.ignore();
    if (pi < 1 || pi > 4) pi = 1;
    Priority prio = (Priority)pi;

    Order o;
    o.id            = generateId(orderCounter);
    o.customerName  = customerName;
    o.customerPhone = customerPhone;
    o.restaurantName= chosen.name;
    o.itemName      = selectedItem.name;
    o.amount        = selectedItem.price;
    o.priority      = prio;
    o.status        = OrderStatus::PENDING;
    o.etaMinutes    = chosen.avgPrepTime + 15;
    o.trackStep     = 0;
    o.timestamp     = getTimestamp();

    clearScreen();
    printReceipt(o);

    cout << "\n  Confirm order? [y/n]: ";
    char c; cin >> c; cin.ignore();
    if (c != 'y' && c != 'Y') { printInfo("Order cancelled."); cout << "\n  [Enter] Back  "; cin.get(); return; }

    allOrders.push_back(o);
    orderHash.insert(o);
    orderQueue.push(o);
    processingQueue.push(o);
    modStack.push({o.id, "Placed by " + customerName + " at " + o.timestamp});

    printSuccess("Order " + o.id + " confirmed!");
    printInfo("Estimated delivery: " + to_string(o.etaMinutes) + " minutes");
    printInfo("Use 'Track my order' to follow your delivery on the map.");
    cout << "\n  [Enter] Continue  "; cin.get();
}

void CustomerPanel::printReceipt(const Order& o) {
    int W = 44;
    string bar(W, '-');

    cout << "\n";
    cout << "  +" << bar << "+\n";
    cout << "  |" << string(W, ' ') << "|\n";
    cout << "  |" << "     FOOD DELIVERY SYSTEM ORDER RECEIPT     " << "|\n";
    cout << "  |" << string(W, ' ') << "|\n";
    cout << "  +" << bar << "+\n";

    auto row = [&](const string& label, const string& val) {
        string line = "  " + label + ": " + val;
        int pad = W - (int)line.size() + 2;
        cout << "  |" << line << string(max(1, pad), ' ') << "|\n";
    };

    row("Order ID   ", o.id);
    row("Customer   ", o.customerName);
    row("Phone      ", o.customerPhone);
    row("Placed at  ", o.timestamp);
    cout << "  |" << string(W, '-') << "|\n";
    row("Restaurant ", o.restaurantName);
    row("Item       ", o.itemName);
    row("Priority   ", priorityStr(o.priority));
    row("ETA        ", to_string(o.etaMinutes) + " minutes");
    cout << "  |" << string(W, '-') << "|\n";

    string amtStr = "Rs " + to_string((int)o.amount) + ".00";
    string amtLine = "  TOTAL: " + amtStr;
    int pad = W - (int)amtLine.size() + 2;
    cout << "  |" << BOLD << amtLine << RESET << string(max(1, pad), ' ') << "|\n";
    cout << "  |" << string(W, ' ') << "|\n";
    cout << "  |" << "      Thank you for ordering with us!    " << "|\n";
    cout << "  |" << string(W, ' ') << "|\n";
    cout << "  +" << bar << "+\n";
}

void CustomerPanel::printDeliveryMap(int step) {
    struct Stage {
        string label;
        string detail;
    };

    Stage stages[] = {
        { "Order Placed",       "Your order has been received"              },
        { "Order Confirmed",    "Restaurant has accepted your order"        },
        { "Preparing",          "Kitchen is preparing your food"            },
        { "Out for Delivery",   "Rider is on the way to your location"      },
        { "Delivered",          "Your order has been delivered. Enjoy!"     },
    };
    const int N = 5;

    time_t now = time(nullptr);
    char datebuf[32];
    strftime(datebuf, sizeof(datebuf), "%A, %d %B", localtime(&now));

    cout << "\n  " << BOLD << datebuf << RESET << "\n\n";

    for (int i = N - 1; i >= 0; i--) {
        bool done    = (i < step);
        bool current = (i == step);

        if (current) {
            cout << "  " << CORAL << BOLD << "  o " << RESET;
        } else if (done) {
            cout << "  " << BGREEN << "  o " << RESET;
        } else {
            cout << "  " << DIM   << "  o " << RESET;
        }

        if (current) {
            cout << BOLD << stages[i].label << RESET << "\n";
            cout << "  " << "  | " << DIM << stages[i].detail << RESET << "\n";
        } else if (done) {
            cout << BGREEN << stages[i].label << RESET << "\n";
            cout << "  " << "  | " << DIM << stages[i].detail << RESET << "\n";
        } else {
            cout << DIM << stages[i].label << RESET << "\n";
            cout << "  " << "  | \n";
        }

        if (i > 0) {
            cout << "  " << "  | \n";
        }
    }

    cout << "\n";

    double pct = (step / 4.0) * 100.0;
    cout << "  ";
    int barW = 40;
    int filled = (int)(pct / 100.0 * barW);
    cout << "[";
    for (int i = 0; i < filled; i++) cout << "\xe2\x96\x88";
    for (int i = filled; i < barW; i++) cout << " ";
    cout << "]  " << BOLD << (int)pct << "%" << RESET << "\n";
}

void CustomerPanel::trackMyOrder() {
    clearScreen();
    cout << "\n";
    printBox("  TRACK MY ORDER", ORANGE);
    cout << "\n  Enter Order ID (e.g. ORD-001): ";
    string id; getline(cin, id);

    Order* o = orderHash.search(id);
    if (!o) {
        printError("Order '" + id + "' not found.");
        cout << "\n  [Enter] Back  "; cin.get(); return;
    }

    if (o->customerName != customerName && o->customerPhone != customerPhone) {
        printWarning("This order is not linked to your account.");
    }

    clearScreen();
    cout << "\n";
    printBox("  ORDER TRACKING  --  " + o->id, ORANGE);
    cout << "\n";
    cout << "  Item      : " << BOLD << o->itemName << RESET
         << "  from  " << o->restaurantName << "\n";
    cout << "  Amount    : Rs " << fixed << setprecision(0) << o->amount << "\n";
    cout << "  Partner   : " << (o->partnerName.empty() ? "Being assigned..." : o->partnerName) << "\n";
    cout << "  ETA       : " << o->etaMinutes << " min\n";
    printLine(68);

    printDeliveryMap(o->trackStep);

    cout << "\n  [Enter] Back  "; cin.get();
}

void CustomerPanel::viewMyOrders() {
    clearScreen();
    cout << "\n";
    printBox("  MY ORDERS  --  " + customerName, CYAN);
    cout << "\n";

    vector<Order> mine;
    for (size_t i = 0; i < allOrders.size(); i++)
        if (allOrders[i].customerName == customerName ||
            allOrders[i].customerPhone == customerPhone)
            mine.push_back(allOrders[i]);

    if (mine.empty()) {
        printInfo("No orders found for your account.");
        cout << "\n  [Enter] Back  "; cin.get(); return;
    }

    printLine(68);
    cout << "  " << left << setw(12) << "ORDER ID"
         << setw(20) << "ITEM"
         << setw(16) << "RESTAURANT"
         << setw(10) << "STATUS"
         << "AMOUNT\n";
    printLine(68);
    for (size_t i = 0; i < mine.size(); i++) {
        const Order& o = mine[i];
        cout << "  " << BOLD << left << setw(12) << o.id << RESET;
        cout << setw(20) << o.itemName;
        cout << setw(16) << o.restaurantName;
        cout << statusColor(o.status) << setw(12) << statusStr(o.status) << RESET;
        cout << "Rs " << fixed << setprecision(0) << o.amount << "\n";
    }
    printLine(68);
    cout << "\n  " << mine.size() << " order(s) found.\n";
    cout << "\n  [Enter] Back  "; cin.get();
}
