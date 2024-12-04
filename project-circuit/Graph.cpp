#include "Graph.h"

int* Graph::dfs(Node& node, Node*& lastNode, bool& foundGround) {
    static int nodeCounter{};

    if (node.visited) return &nodeCounter;

    node.visited = true;

    if (!node.isGround)
        node.value = nodeCounter++;
    else
        foundGround = true;

    lastNode = &node;

    for (auto& edge : node.edges) {
         dfs(*(edge.node), lastNode,  foundGround);
    }
    
    return &nodeCounter;
}