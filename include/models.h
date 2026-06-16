#pragma once
#include <string>
#include "ui.h"

enum class OrderStatus {
    PENDING, CONFIRMED, PREPARING, ON_THE_WAY, DELIVERED, CANCELLED
};

enum class Priority { LOW = 1, MEDIUM = 2, HIGH = 3, URGENT = 4 };

std::string statusStr(OrderStatus s);
std::string statusColor(OrderStatus s);
std::string priorityStr(Priority p);
std::string priorityColor(Priority p);

struct Order {
    std::string id;
    std::string customerName;
    std::string customerPhone;
    std::string restaurantName;
    std::string itemName;
    double      amount;
    Priority    priority;
    OrderStatus status;
    std::string partnerName;
    std::string timestamp;
    int         etaMinutes;
    int         trackStep;

    Order();
    Order(std::string i, std::string c, std::string ph,
          std::string r, std::string item,
          double amt, Priority p, int eta);

    bool operator<(const Order& o) const;
};

void printOrderRow(const Order& o, bool showDetails = false);

struct MenuItem {
    std::string name;
    std::string description;
    double      price;
    std::string category;
};

struct Restaurant {
    std::string name;
    std::string cuisine;
    double      rating;
    int         totalOrders;
    double      revenue;
    int         avgPrepTime;
    bool        isOpen;
    std::string address;

    Restaurant();
    Restaurant(std::string n, std::string c, double r,
               int orders, double rev, int prep,
               bool open, std::string addr);
};

void printRestRow(const Restaurant& r);

struct DeliveryPartner {
    std::string id;
    std::string name;
    std::string vehicle;
    double      rating;
    int         totalDeliveries;
    bool        isAvailable;
    std::string currentZone;
    double      earnings;

    DeliveryPartner();
    DeliveryPartner(std::string i, std::string n, std::string v,
                    double r, int d, bool avail,
                    std::string z, double e);
};
