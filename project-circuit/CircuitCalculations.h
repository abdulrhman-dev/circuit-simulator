#ifndef CIRCUIT_CALCULATIONS_H
#define CIRCUIT_CALCULATIONS_H
#include "Output.h"
#include <list>

float calculateVoltageDiff(const CircuitElement& circuitElement);
bool SolveCircuit(std::list<NodeObject>& nodes, std::list<CircuitElement>& circuitElements);
#endif // !CIRCUIT_CALCULATIONS_H


