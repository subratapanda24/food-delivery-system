#pragma once
#include "models.h"
#include <vector>
#include <string>

class OrderHashTable {
private:
    static const int       TABLE_SIZE = 16;
    std::vector<std::vector<Order>> table;
    int                    count;

    int hashFn(const std::string& key);

public:
    OrderHashTable();

    void   insert(const Order& o);
    Order* search(const std::string& id);
    bool   remove(const std::string& id);
    void   update(const std::string& id, OrderStatus status);
    void   updateTrackStep(const std::string& id, int step);
    void   printTable();
    int    getCount();
};
