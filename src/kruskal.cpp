#include "kruskal.hpp"

#include <algorithm>

using namespace std;

UnionFind::UnionFind(int n) {

    parent.resize(n);
    rank.resize(n, 0);

    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
}

int UnionFind::find(int x) {

    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }

    return parent[x];
}

bool UnionFind::unite(int x, int y) {

    int rootX = find(x);
    int rootY = find(y);

    if (rootX == rootY) {
        return false;
    }

    if (rank[rootX] < rank[rootY]) {

        parent[rootX] = rootY;

    }
    else if (rank[rootX] > rank[rootY]) {

        parent[rootY] = rootX;

    }
    else {

        parent[rootY] = rootX;
        rank[rootX]++;
    }

    return true;
}

vector<Edge> kruskalMST(Graph& graph, int& totalWeight) {

    vector<Edge> mst;

    totalWeight = 0;

    //Ordenar aristar por pesos
    sort(graph.edges.begin(), graph.edges.end(),
        [](const Edge& a, const Edge& b) {
            return a.weight < b.weight;
        }
    );


    UnionFind uf(graph.numNodes);

    for (const Edge& edge : graph.edges) {

        if (uf.unite(edge.u, edge.v)) {

            mst.push_back(edge);

            totalWeight += edge.weight;
        }

        if (mst.size() == graph.numNodes - 1) {
            break;
        }
    }

    return mst;
}