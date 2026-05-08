#include <iostream>
#include <fstream>
#include <vector>

#include "parser.hpp"
#include "mergesort.hpp"
#include "binary_search.hpp"

#include "graph.hpp"
#include "kruskal.hpp"

using namespace std;

int main() {


    // MODULO A

    cout << "===== MODULO A =====" << endl;

    //parsear csv

    ParseResult res = parseCSV(
        "data/WA_Fn-UseC_-Telco-Customer-Churn.csv"
    );

    cout << "Registros cargados: "
         << res.totalRegistros
         << endl;

    cout << "TotalCharges nulos: "
         << res.totalChargesNulos
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

    cout << "\nPipeline completado correctamente."
         << endl;

    return 0;
}