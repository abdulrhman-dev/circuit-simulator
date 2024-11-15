#include "Graph.h"

namespace Graph {
    void dfs(Node& node, int value) {
        if (node.visited) return;

        node.visited = true;
        node.value = value;

        for (auto& edge : node.edges) {
            if (edge.traverse)
                dfs(edge.node, value);
        }
    }
}