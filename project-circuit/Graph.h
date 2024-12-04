#pragma once
#include <vector>
#include <list>

namespace Graph {
    struct Node;

    struct Edge {
        Node* node;
        bool traverse{};
    };

    struct Node {
        int value{-1};
        std::vector<Edge> edges;
        bool visited{};
        bool isGround{};
        int id{0};
    };


    int* dfs(Node& node, Node*& lastNode, bool& foundGround);
}
