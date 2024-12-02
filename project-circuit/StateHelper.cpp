#include "StateHelper.h"

void deleteNode(std::list<NodeObject>& nodes, int nodeGraphID) {
    auto deleteElement = std::find_if(nodes.begin(), nodes.end(), [nodeGraphID](const NodeObject& node) {
        return node.graphNode.id == nodeGraphID;
        });

    if (deleteElement != nodes.end())
        nodes.erase(deleteElement);
}

void deleteEdge(std::vector<Graph::Edge>& edges, int nodeGraphID) {
    auto deleteElement = std::find_if(edges.begin(), edges.end(), [nodeGraphID](const Graph::Edge& edge) {
        return edge.node->id == nodeGraphID;
        });

    if (deleteElement != edges.end())
        edges.erase(deleteElement);
}

void deleteElement(const CircuitElementIterator& deletedELement, std::list<CircuitElement>& circuitElements, std::list<NodeObject>& nodes) {
    if (circuitElements.size() < 1)
        return;

    auto& startNodeEdges = (*deletedELement).startNode->graphNode.edges;
    auto& endNodeEdges = (*deletedELement).endNode->graphNode.edges;

    int startNodeID = (*deletedELement).startNode->graphNode.id;
    int endNodeID = (*deletedELement).endNode->graphNode.id;


    deleteEdge(startNodeEdges, endNodeID);
    deleteEdge(endNodeEdges, startNodeID);


    if (startNodeEdges.size() == 0)
        deleteNode(nodes, startNodeID);

    if (endNodeEdges.size() == 0)
        deleteNode(nodes, endNodeID);

    circuitElements.erase(deletedELement);
}