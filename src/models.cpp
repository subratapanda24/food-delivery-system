#include "../include/models.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

string statusStr(OrderStatus s) {
    switch (s) {
        case OrderStatus::PENDING:    return "PENDING";
        case OrderStatus::CONFIRMED:  return "CONFIRMED";
        case OrderStatus::PREPARING:  return "PREPARING";
        case OrderStatus::ON_THE_WAY: return "ON THE WAY";
        case OrderStatus::DELIVERED:  return "DELIVERED";
        case OrderStatus::CANCELLED:  return "CANCELLED";
        default: return "UNKNOWN";
    }
}

string statusColor(OrderStatus s) {
    switch (s) {
        case OrderStatus::PENDING:    return YELLOW;
        case OrderStatus::CONFIRMED:  return CYAN;
        case OrderStatus::PREPARING:  return CYAN;
        case OrderStatus::ON_THE_WAY: return BGREEN;
        case OrderStatus::DELIVERED:  return GREEN;
        case OrderStatus::CANCELLED:  return BRED;
        default: return WHITE;
    }
}

string priorityStr(Priority p) {
    switch (p) {
        case Priority::LOW:    return "LOW";
        case Priority::MEDIUM: return "MED";
        case Priority::HIGH:   return "HIGH";
        case Priority::URGENT: return "URGENT";
        default: return "?";
    }
}

string priorityColor(Priority p) {
    switch (p) {
        case Priority::LOW:    return DIM;
        case Priority::MEDIUM: return WHITE;
        case Priority::HIGH:   return YELLOW;
        case Priority::URGENT: return BRED;
        default: return WHITE;
    }
}

Order::Order()
    : amount(0), priority(Priority::MEDIUM),
      status(OrderStatus::PENDING), etaMinutes(30), trackStep(0) {}

Order::Order(string i, string c, string ph,
             string r, string item,
             double amt, Priority p, int eta)
    : id(i), customerName(c), customerPhone(ph), restaurantName(r),
      itemName(item), amount(amt), priority(p),
      status(OrderStatus::PENDING), etaMinutes(eta), trackStep(0)
{
    timestamp = getTimestamp();
}

bool Order::operator<(const Order& o) const {
    return (int)priority < (int)o.priority;
}

void printOrderRow(const Order& o, bool showDetails) {
    string sc = statusColor(o.status);
    string pc = priorityColor(o.priority);
    cout << "  " << BOLD << left << setw(12) << o.id << RESET;
    cout << left << setw(18) << o.customerName;
    cout << left << setw(16) << o.restaurantName;
    cout << pc << BOLD << setw(8) << priorityStr(o.priority) << RESET;
    cout << sc << setw(12) << statusStr(o.status) << RESET;
    cout << "Rs" << fixed << setprecision(0) << o.amount;
    cout << "\n";
    if (showDetails) {
        cout << "  " << DIM
             << "  Item: " << o.itemName
             << "  |  ETA: " << o.etaMinutes << "min"
             << "  |  Partner: "
             << (o.partnerName.empty() ? "Unassigned" : o.partnerName)
             << "  |  Placed: " << o.timestamp
             << RESET << "\n";
    }
}

Restaurant::Restaurant()
    : rating(0), totalOrders(0), revenue(0), avgPrepTime(20), isOpen(true) {}

Restaurant::Restaurant(string n, string c, double r,
                       int orders, double rev, int prep,
                       bool open, string addr)
    : name(n), cuisine(c), rating(r), totalOrders(orders), revenue(rev),
      avgPrepTime(prep), isOpen(open), address(addr) {}

void printRestRow(const Restaurant& r) {
    cout << "  " << BOLD << left << setw(20) << r.name << RESET;
    cout << left << setw(16) << r.cuisine;
    cout << YELLOW << "* " << fixed << setprecision(1) << r.rating << RESET;
    cout << setw(12) << ("  " + to_string(r.totalOrders) + " orders");
    cout << (r.isOpen ? (string(GREEN) + "  OPEN" + RESET)
                      : (string(BRED)  + "  CLOSED" + RESET));
    cout << "\n";
}

DeliveryPartner::DeliveryPartner()
    : rating(0), totalDeliveries(0), isAvailable(true), earnings(0) {}

DeliveryPartner::DeliveryPartner(string i, string n, string v,
                                 double r, int d, bool avail,
                                 string z, double e)
    : id(i), name(n), vehicle(v), rating(r), totalDeliveries(d),
      isAvailable(avail), currentZone(z), earnings(e) {}
