#include "knapsack.hpp"
#include <cmath>
#include <algorithm>

using namespace std;

static inline void toWeightValue(const Solicitud& s, int &w, int &v) {
    w = int(std::lround(s.totalCharges)); // peso: TotalCharges redondeado al entero más cercano
    v = int(std::lround(s.monthlyCharges * 10.0)); // valor: MonthlyCharges * 10 redondeado (centavos)
    if (w < 0) w = 0;
    if (v < 0) v = 0;
}

KSResult solveKnapsackDP(const vector<Solicitud>& items, int W) {
    int n = (int)items.size();
    KSResult res;
    res.dp.assign(n + 1, vector<int>(W + 1, 0));

    // Construir tabla DP
    for (int i = 1; i <= n; ++i) {
        int wi, vi;
        toWeightValue(items[i-1], wi, vi);
        for (int w = 0; w <= W; ++w) {
            if (wi <= w) {
                res.dp[i][w] = max(res.dp[i-1][w], res.dp[i-1][w-wi] + vi);
            } else {
                res.dp[i][w] = res.dp[i-1][w];
            }
        }
    }

    // Valor óptimo
    res.totalValue = res.dp[n][W];
    res.totalWeight = 0;

    return res;
}
