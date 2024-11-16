#ifndef GRAPH_H
#define GRAPH_H
#include "vector"
namespace Graph {
    struct Node;

    struct Edge {
        Node& node;
        bool traverse{};
    };

    struct Node {
        int value{};
        std::vector<Edge> edges;
        bool visited{};
    };


    void dfs(Node& node, int value);
}
#endif // !GRAPH_H