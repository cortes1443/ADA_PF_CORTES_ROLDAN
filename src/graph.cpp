#include "graph.hpp"

#include <cmath>
#include <vector>

using namespace std;

Graph buildGraph(const vector<Solicitud>& solicitudes) {

    Graph graph;
    graph.numNodes = 20;

 
    vector<double> sumCharges(20, 0.0);
    vector<int> counts(20, 0);

    //agrupar las solicitudes por mod 20
    for (int i = 0; i < solicitudes.size(); i++) {

        int group = i % 20;

        sumCharges[group] += solicitudes[i].monthlyCharges;
        counts[group]++;
    }


    vector<double> averages(20, 0.0);

    for (int i = 0; i < 20; i++) {

        if (counts[i] > 0) {
            averages[i] = round((sumCharges[i] / counts[i]) * 100.0) / 100.0;
        }
    }


    //crear grafo
    for (int u = 0; u < 20; u++) {

        for (int v = u + 1; v < 20; v++) {

            Edge edge;

            edge.u = u;
            edge.v = v;

            // floor(Mu + Mv)
            edge.weight = floor(averages[u] + averages[v]);

            graph.edges.push_back(edge);
        }
    }

    return graph;
}