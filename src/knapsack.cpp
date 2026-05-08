#include "knapsack.hpp"
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <filesystem>

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

    // Construir tabla PD
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

    // Backtracking para reconstruir selección
    int w = W;
    res.totalWeight = 0;
    for (int i = n; i >= 1; --i) {
        if (res.dp[i][w] != res.dp[i-1][w]) {
            // se tomó el item i-1
            res.selectedIndices.push_back(i-1);
            int wi, vi;
            toWeightValue(items[i-1], wi, vi);
            res.totalWeight += wi;
            w -= wi;
        }
    }
    reverse(res.selectedIndices.begin(), res.selectedIndices.end());

    return res;
}

vector<int> greedyByRatio(const vector<Solicitud>& items, int W) {
    int n = (int)items.size();
    vector<pair<double,int>> order; order.reserve(n);
    for (int i = 0; i < n; ++i) {
        int wi, vi; toWeightValue(items[i], wi, vi);
        double ratio = (wi == 0) ? (vi > 0 ? 1e9 : 0.0) : (double)vi / (double)wi;
        order.emplace_back(-ratio, i); // negativo para ordenar descendente por ratio
    }
    sort(order.begin(), order.end());

    vector<int> sel;
    int used = 0;
    for (auto &p : order) {
        int idx = p.second;
        int wi, vi; toWeightValue(items[idx], wi, vi);
        if (used + wi <= W) {
            sel.push_back(idx);
            used += wi;
        }
    }
    sort(sel.begin(), sel.end());
    return sel;
}

// calcula valor total y peso de un conjunto de índices sobre items
static void computeSetStats(const vector<Solicitud>& items, const vector<int>& sel, int& totalW, int& totalV) {
    totalW = 0; totalV = 0;
    for (int idx : sel) {
        int wi, vi; toWeightValue(items[idx], wi, vi);
        totalW += wi;
        totalV += vi;
    }
}

// Evalúa PD exacta para un subconjunto de items
static pair<vector<int>,int> exactOnSubset(const vector<Solicitud>& subset, int W) {
    KSResult r = solveKnapsackDP(subset, W);
    return {r.selectedIndices, r.totalValue};
}

bool findGreedyCounterexample3(const vector<Solicitud>& items, int W,
                               vector<int>& greedySel, vector<int>& optimalSel) {
    int n = (int)items.size();
    // iterar combinaciones de 3 índices
    for (int a = 0; a < n; ++a) {
        for (int b = a+1; b < n; ++b) {
            for (int c = b+1; c < n; ++c) {
                vector<Solicitud> subset = {items[a], items[b], items[c]};
                // greedy sobre subset
                vector<int> localGreedy = greedyByRatio(subset, W);
                int gW, gV; computeSetStats(subset, localGreedy, gW, gV);
                auto [optSelLocal, optV] = exactOnSubset(subset, W);
                if (gV != optV) {
                    // traducir índices locales a globales
                    greedySel.clear(); optimalSel.clear();
                    for (int idx : localGreedy) {
                        if (idx == 0) greedySel.push_back(a);
                        else if (idx == 1) greedySel.push_back(b);
                        else greedySel.push_back(c);
                    }
                    for (int idx : optSelLocal) {
                        if (idx == 0) optimalSel.push_back(a);
                        else if (idx == 1) optimalSel.push_back(b);
                        else optimalSel.push_back(c);
                    }
                    sort(greedySel.begin(), greedySel.end());
                    sort(optimalSel.begin(), optimalSel.end());
                    return true;
                }
            }
        }
    }
    return false;
}

void writeAssignmentReport(const string& filename, const KSResult& res,
                           const vector<Solicitud>& items,
                           const vector<int>& greedySel,
                           const vector<int>& counterGreedy,
                           const vector<int>& counterOptimal,
                           int W) {
    filesystem::path path(filename);
    if (path.has_parent_path()) {
        filesystem::create_directories(path.parent_path());
    }

    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "No se pudo crear " << filename << "\n";
        return;
    }

    out << "Tabla PD (filas i=0..n, columnas w=0..W)\n";
    int n = (int)res.dp.size() - 1;
    for (int i = 0; i <= n; ++i) {
        for (int w = 0; w <= W; ++w) {
            out << res.dp[i][w];
            if (w < W) out << ',';
        }
        out << '\n';
    }

    out << "\nSolucion optima (PD):\n";
    out << "Valor total (en centavos): " << res.totalValue << "\n";
    out << "Peso total usado: " << res.totalWeight << "\n";
    out << "Numero solicitudes seleccionadas: " << res.selectedIndices.size() << "\n";
    out << "CustomerIDs seleccionados:\n";
    for (int idx : res.selectedIndices) {
        out << items[idx].customerID << '\n';
    }

    out << "\nSeleccion voraz por ratio:\n";
    for (int idx : greedySel) {
        out << items[idx].customerID << ',';
    }
    out << '\n';

    out << "\nContraejemplo codicioso (3 items):\n";
    if (counterOptimal.empty()) {
        out << "No se encontro contraejemplo de 3 items.\n";
    } else {
        out << "Greedy seleccion: ";
        for (int idx : counterGreedy) {
            out << items[idx].customerID << ',';
        }
        out << '\n';
        out << "Optimo PD: ";
        for (int idx : counterOptimal) {
            out << items[idx].customerID << ',';
        }
        out << '\n';
    }

    out << "\nAnalisis de complejidad:\n";
    out << "Tiempo: Theta(n * W) porque se llena una tabla de n filas por W columnas.\n";
    out << "Espacio: Theta(n * W) porque se almacena la tabla completa dp.\n";

    out.close();
}
