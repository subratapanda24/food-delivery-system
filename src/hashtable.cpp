#include "../include/hashtable.h"
#include "../include/ui.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Hash table
int OrderHashTable::hashFn(const string& key) {
    int h = 0;
    for (size_t i = 0; i < key.size(); i++)
        h = (h * 31 + key[i]) % TABLE_SIZE;
    return h;
}

OrderHashTable::OrderHashTable() : table(TABLE_SIZE), count(0) {}

void OrderHashTable::insert(const Order& o) {
    int idx = hashFn(o.id);
    for (size_t i = 0; i < table[idx].size(); i++) {
        if (table[idx][i].id == o.id) { table[idx][i] = o; return; }
    }
    table[idx].push_back(o);
    count++;
}

Order* OrderHashTable::search(const string& id) {
    int idx = hashFn(id);
    for (size_t i = 0; i < table[idx].size(); i++)
        if (table[idx][i].id == id) return &table[idx][i];
    return nullptr;
}

bool OrderHashTable::remove(const string& id) {
    int idx = hashFn(id);
    vector<Order>& bucket = table[idx];
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
        if (it->id == id) { bucket.erase(it); count--; return true; }
    }
    return false;
}

void OrderHashTable::update(const string& id, OrderStatus status) {
    Order* o = search(id);
    if (o) o->status = status;
}

void OrderHashTable::updateTrackStep(const string& id, int step) {
    Order* o = search(id);
    if (o) o->trackStep = step;
}

void OrderHashTable::printTable() {
    cout << "\n";
    for (int i = 0; i < TABLE_SIZE; i++) {
        cout << "  [" << setw(2) << i << "]  ";
        if (table[i].empty()) {
            cout << DIM << "(empty)" << RESET;
        } else {
            for (int j = 0; j < (int)table[i].size(); j++) {
                cout << BOLD << table[i][j].id << RESET;
                if (j < (int)table[i].size()-1) cout << " -> ";
            }
        }
        cout << "\n";
    }
    cout << "\n  " << DIM << "Load factor: " << fixed << setprecision(2)
         << (double)count / TABLE_SIZE
         << "  (" << count << " / " << TABLE_SIZE << " slots used)" << RESET << "\n";
}

int OrderHashTable::getCount() { return count; }
