#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "parser.hpp"
#include "mergesort.hpp"
#include "binary_search.hpp"

#include "graph.hpp"
#include "kruskal.hpp"
#include "knapsack.hpp"

using namespace std;

static void benchmarkModuleA(const vector<Solicitud>& solicitudesOrdenadas) {
    struct SampleCase {
        string label;
        size_t size;
    };

    const vector<SampleCase> cases = {
        {"Pequeno", 100},
        {"Mediano", 1000},
        {"Grande", 5000},
    };

    ofstream timesOut("results/tiempos_modulo_A.txt");
    timesOut << "Tamano de muestra,MergeSort_ms,BusquedaBinaria_ms\n";

    const vector<int> queries = {72, 60, 45, 30, 12};

    cout << "\n===== TIEMPOS MODULO A =====" << endl;

    for (const auto& sampleCase : cases) {
        size_t sampleSize = min(sampleCase.size, solicitudesOrdenadas.size());
        vector<Solicitud> sample(solicitudesOrdenadas.begin(), solicitudesOrdenadas.begin() + sampleSize);

        constexpr int repetitions = 100;

        auto sortStart = chrono::steady_clock::now();
        for (int rep = 0; rep < repetitions; ++rep) {
            vector<Solicitud> temp = sample;
            mergeSort(temp, 0, temp.size() - 1);
        }
        auto sortEnd = chrono::steady_clock::now();

        auto searchStart = chrono::steady_clock::now();
        for (int rep = 0; rep < repetitions; ++rep) {
            for (int k : queries) {
                (void)binarySearchFirstGE(sample, 0, sample.size() - 1, k);
            }
        }
        auto searchEnd = chrono::steady_clock::now();

        double sortMs = chrono::duration<double, milli>(sortEnd - sortStart).count() / repetitions;
        double searchMs = chrono::duration<double, milli>(searchEnd - searchStart).count() / repetitions;

        cout << sampleCase.label << ": MergeSort=" << sortMs
             << " ms, BusquedaBinaria=" << searchMs << " ms" << endl;

        timesOut << sampleCase.label << "," << sortMs << "," << searchMs << "\n";
    }

    timesOut.close();
}

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

    benchmarkModuleA(res.solicitudes);

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

    int count_churn_false = 0, count_churn_true = 0;
    for (const auto &s : res.solicitudes) {
        if (!s.churn) count_churn_false++;
        else count_churn_true++;
    }

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