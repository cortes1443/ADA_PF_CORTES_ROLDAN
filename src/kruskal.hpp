#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include <vector>
#include "graph.hpp"


class UnionFind {

private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    UnionFind(int n);

    int find(int x);

    bool unite(int x, int y);
};


std::vector<Edge> kruskalMST(Graph& graph, int& totalWeight);

#endif