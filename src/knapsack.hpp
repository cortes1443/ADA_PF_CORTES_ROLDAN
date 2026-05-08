#ifndef KNAPSACK_HPP
#define KNAPSACK_HPP

#include <vector>
#include <string>
#include "parser.hpp"

struct KSResult {
    int totalValue; // valor óptimo (en centavos)
    int totalWeight; // peso total utilizado
    std::vector<int> selectedIndices; // índices de solicitudes seleccionadas (0-based)
    std::vector<std::vector<int>> dp; // tabla DP para inspección / salida
};

// Resuelve la mochila 0-1 por programación dinámica. Devuelve la tabla DP y la solución.
KSResult solveKnapsackDP(const std::vector<Solicitud>& items, int W);

// Selección voraz por ratio (value/weight). Devuelve índices seleccionados.
std::vector<int> greedyByRatio(const std::vector<Solicitud>& items, int W);

// Busca un contraejemplo usando exactamente 3 ítems del conjunto: devuelve true si lo encuentra
bool findGreedyCounterexample3(const std::vector<Solicitud>& items, int W,
                               std::vector<int>& greedySel, std::vector<int>& optimalSel);

#endif
