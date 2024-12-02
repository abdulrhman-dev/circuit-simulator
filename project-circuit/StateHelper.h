#pragma once
#include "Output.h"
using CircuitElementIterator = std::list<CircuitElement>::iterator;

void deleteNode(std::list<NodeObject>& nodes, int nodeGraphID);

void deleteEdge(std::vector<Graph::Edge>& edges, int nodeGraphID);

void deleteElement(const CircuitElementIterator& deletedELement, std::list<CircuitElement>& circuitElements, std::list<NodeObject>& nodes);