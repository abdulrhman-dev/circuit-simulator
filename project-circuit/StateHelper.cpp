#include "StateHelper.h"
#include <iostream>

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

void deleteGroundsConnected(std::list<CircuitElement>& circuitElements, int nodeGraphID) {
    circuitElements.resize(std::distance(
        circuitElements.begin(),
        std::remove_if(circuitElements.begin(), circuitElements.end(), [nodeGraphID](const CircuitElement& circuitElement) 
            {
                return circuitElement.startNode->graphNode.id == nodeGraphID && circuitElement.state == DrawState::GROUND;
            }
        )
    ));
}

void deleteGround(const CircuitElementIterator& deletedElement, std::list<CircuitElement>& circuitElements) {
    NodeObject* groundNode = (*deletedElement).startNode;
    circuitElements.erase(deletedElement);

    auto found = std::find_if(circuitElements.begin(), circuitElements.end(),
        [groundNodeID = groundNode->graphNode.id](const CircuitElement& circuitElement) {
            return circuitElement.startNode->graphNode.id == groundNodeID && circuitElement.state == DrawState::GROUND;
        }
    );

    if (found == circuitElements.end()) {
        groundNode->isGround = false;
    }
}

void deleteElement(const CircuitElementIterator& deletedELement, std::list<CircuitElement>& circuitElements, std::list<NodeObject>& nodes) {
    if (circuitElements.size() < 1)
        return;

    if (deletedELement->state == DrawState::GROUND) {
        deleteGround(deletedELement, circuitElements);
        return;
    }

    auto& startNodeEdges = (*deletedELement).startNode->graphNode.edges;
    auto& endNodeEdges = (*deletedELement).endNode->graphNode.edges;

    int startNodeID = (*deletedELement).startNode->graphNode.id;
    int endNodeID = (*deletedELement).endNode->graphNode.id;


    deleteEdge(startNodeEdges, endNodeID);
    deleteEdge(endNodeEdges, startNodeID);

    // these variables are stored before the iterator gets invalidated
    bool cleanUpStartNodes = startNodeEdges.size() == 0;
    bool cleanUpSEndNodes = endNodeEdges.size() == 0;

    circuitElements.erase(deletedELement);

    if (cleanUpStartNodes) {
        deleteGroundsConnected(circuitElements, startNodeID);
        deleteNode(nodes, startNodeID);
    }

    if (cleanUpSEndNodes) {
        deleteGroundsConnected(circuitElements, endNodeID);
        deleteNode(nodes, endNodeID);
    }
}