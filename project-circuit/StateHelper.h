#pragma once
#include "NodeObject.h"
#include "CircuitElement.h"
using CircuitElementIterator = std::list<CircuitElement>::iterator;

void deleteElement(const CircuitElementIterator& deletedELement, std::list<CircuitElement>& circuitElements, std::list<NodeObject>& nodes);