#include "../include/sorting.h"
#include <algorithm>

using namespace std;

// Merge sort
static void merge(vector<Order> &arr, int l, int mid, int r,
                  function<bool(const Order &, const Order &)> cmp)
{
    vector<Order> left(arr.begin() + l, arr.begin() + mid + 1);
    vector<Order> right(arr.begin() + mid + 1, arr.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < (int)left.size() && j < (int)right.size())
        arr[k++] = cmp(left[i], right[j]) ? left[i++] : right[j++];
    while (i < (int)left.size())
        arr[k++] = left[i++];
    while (j < (int)right.size())
        arr[k++] = right[j++];
}

void mergeSort(vector<Order> &arr, int l, int r,
               function<bool(const Order &, const Order &)> cmp)
{
    if (l >= r)
        return;
    int mid = l + (r - l) / 2;
    mergeSort(arr, l, mid, cmp);
    mergeSort(arr, mid + 1, r, cmp);
    merge(arr, l, mid, r, cmp);
}

// Quick sort
static int partition(vector<Order> &arr, int low, int high,
                     function<bool(const Order &, const Order &)> cmp)
{
    Order pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++)
        if (cmp(arr[j], pivot))
            swap(arr[++i], arr[j]);
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<Order> &arr, int low, int high,
               function<bool(const Order &, const Order &)> cmp)
{
    if (low < high)
    {
        int pi = partition(arr, low, high, cmp);
        quickSort(arr, low, pi - 1, cmp);
        quickSort(arr, pi + 1, high, cmp);
    }
}

// Binary search
int binarySearchRestaurant(const vector<Restaurant> &arr, const string &name)
{
    int lo = 0, hi = (int)arr.size() - 1;
    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid].name == name)
            return mid;
        if (arr[mid].name < name)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return -1;
}
