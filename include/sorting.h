#pragma once
#include "models.h"
#include <vector>
#include <functional>
#include <string>

void mergeSort(std::vector<Order>& arr, int l, int r,
               std::function<bool(const Order&, const Order&)> cmp);

void quickSort(std::vector<Order>& arr, int low, int high,
               std::function<bool(const Order&, const Order&)> cmp);

int binarySearchRestaurant(const std::vector<Restaurant>& arr,
                           const std::string& name);
