#pragma once
#include "CircuitElement.h"
#include "NodeObject.h"
#include <list>

float calculateVoltageDiff(const CircuitElement& circuitElement);
bool SolveCircuit(std::list<NodeObject>& nodes, std::list<CircuitElement>& circuitElements);



