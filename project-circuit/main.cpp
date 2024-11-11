#include <iostream>
#include <vector>
// GUI to Netlist
// Node
// Edge
// CircuitElement
// CircuitGraph

// GUI
// NodeObject
// CircuitElementObject
    // - Resistor
    // - Wire
    // - Voltage Source
    // - Current Source

struct Node;

struct Edge {
    Node& node;
    bool traverse{};
    std::string ID{};
    bool visited{};
};

struct Node {
    int value{};
    std::vector<Edge> edges;
    bool visited{};
};



void dfs(Node& node) {
    if (node.visited) return;

    node.visited = true;

    for (auto& edge : node.edges) {
        if (!edge.visited)
            std::cout << edge.ID << '\n';
      edge.visited = true;
      dfs(edge.node);
    }


}

int main(void)
{
    Node node0{ 0 };
    Node node1{ 1 };
    Node node2{ 2 };


    node0.edges = std::vector<Edge>{ {Edge{node1, true, "01"}, Edge{node2, false, "02"}}};
    node1.edges = std::vector<Edge> { {Edge{node2, true, "12"}, Edge{node0, false, "10"}}};
    node2.edges = std::vector<Edge>{ {Edge{node0, true, "20"}, Edge{node1, false, "21"}}};


    std::vector<std::reference_wrapper<Node>> nodes{ node0, node1, node2 };


    dfs(nodes[0]);
   


    /*for (auto& node : nodes) {
        std::cout << node.get().value << '\n';
    }*/
    return 0;
}
