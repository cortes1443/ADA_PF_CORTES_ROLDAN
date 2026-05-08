#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include "parser.hpp"

struct Edge {
    int u;
    int v;
    int weight;
};

struct Graph {
    int numNodes;
    std::vector<Edge> edges;
};

Graph buildGraph(const std::vector<Solicitud>& solicitudes);

#endif