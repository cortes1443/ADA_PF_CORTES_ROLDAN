#include "binary_search.hpp"

using namespace std;

int binarySearchFirstGE(const vector<Solicitud>& arr, int left, int right, int k) {
    if (left > right) return -1;

    int mid = left + (right - left) / 2;

    if (arr[mid].tenure >= k) {

        int leftResult = binarySearchFirstGE(arr, left, mid - 1, k);

        if (leftResult != -1)
            return leftResult;

        return mid;
    }
    else {
        return binarySearchFirstGE(arr, mid + 1, right, k);
    }
}