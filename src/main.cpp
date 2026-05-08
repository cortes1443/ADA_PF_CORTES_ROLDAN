#include <iostream>
#include <fstream>
#include <vector>

#include "parser.hpp"
#include "mergesort.hpp"
#include "binary_search.hpp"

#include "graph.hpp"
#include "kruskal.hpp"
#include "knapsack.hpp"

using namespace std;

int main() {


    // MODULO A

    cout << "===== MODULO A =====" << endl;

    //parsear csv

    ParseResult res = parseCSV(
        "data/WA_Fn-UseC_-Telco-Customer-Churn.csv"
    );

        cout << "Registros cargados: "
            << res.totalRegisters
            << endl;

        cout << "TotalCharges nulos: "
            << res.totalChargesNulls
            << endl;

    //ordenar por tenure descendente

    mergeSort(
        res.solicitudes,
        0,
        res.solicitudes.size() - 1
    );

    cout << "Solicitudes ordenadas." << endl;

    //exportar csv ordenado

    writeCSV(
        "results/solicitudes_ordenadas.csv",
        res.solicitudes
    );

    cout << "CSV exportado." << endl;

    //busquedas
    vector<int> queries = {72, 60, 45, 30, 12};

    ofstream searchOut("results/busquedas_A.txt");

    for (int k : queries) {

        int idx = binarySearchFirstGE(
            res.solicitudes,
            0,
            res.solicitudes.size() - 1,
            k
        );

        if (idx != -1) {

            searchOut << "k = "
                      << k
                      << " -> "
                      << res.solicitudes[idx].customerID
                      << " (tenure = "
                      << res.solicitudes[idx].tenure
                      << ")"
                      << "\n";
        }
        else {

            searchOut << "k = "
                      << k
                      << " -> No encontrado\n";
        }
    }

    searchOut.close();

    cout << "Busqueda binaria completada." << endl;


    // MODULO B
 
    cout << "\n===== MODULO B =====" << endl;

    //construir grafo

    Graph graph = buildGraph(res.solicitudes);

    cout << "Nodos: "
         << graph.numNodes
         << endl;

    cout << "Aristas: "
         << graph.edges.size()
         << endl;

    //kruskal MST

    int totalWeight = 0;

    vector<Edge> mst = kruskalMST(
        graph,
        totalWeight
    );

    cout << "Aristas MST: "
         << mst.size()
         << endl;

    cout << "Peso total MST: "
         << totalWeight
         << endl;

    //exportar txt del MST

    ofstream mstOut("results/mst_red.txt");

    mstOut << "Minimum Spanning Tree (Kruskal)\n\n";

    for (const Edge& e : mst) {

        mstOut << e.u
               << " - "
               << e.v
               << " : "
               << e.weight
               << "\n";
    }

    mstOut << "\nTotal Weight: "
           << totalWeight
           << "\n";

    mstOut.close();

    cout << "MST exportado." << endl;

    //MODULO C
    cout << "\n===== MODULO C =====" << endl;

    // Tomar las primeras 50 solicitudes activas (Churn == No) del arreglo ya ordenado por tenure
    std::vector<Solicitud> itemsC;
    for (const auto &s : res.solicitudes) {
        if (!s.churn) {
            itemsC.push_back(s);
            if (itemsC.size() >= 50) break;
        }
    }

    int W = 500;
    KSResult ksres = solveKnapsackDP(itemsC, W);
    auto greedy = greedyByRatio(itemsC, W);
    std::vector<int> counterGreedy, counterOptimal;
    bool found = findGreedyCounterexample3(itemsC, W, counterGreedy, counterOptimal);

    // Escribir resultado final en results/asignacion_bw.txt
    writeAssignmentReport("results/asignacion_bw.txt", ksres, itemsC, greedy, counterGreedy, counterOptimal, W);

    cout << "Modulo C completado: valor_opt_centavos=" << ksres.totalValue
         << " items_seleccionados=" << ksres.selectedIndices.size()
         << " contraejemplo3=" << (found?"Si":"No") << endl;

    cout << "\nPipeline completado correctamente." << endl;

    return 0;
}