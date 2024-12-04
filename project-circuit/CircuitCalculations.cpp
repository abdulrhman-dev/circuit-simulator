#include "CircuitCalculations.h"
#include "Circuit.h"
#include "UIConstants.h"
#include "NodeObject.h"
#include "CircuitElement.h"
#include "Graph.h"
#include <memory>
#include <iostream>

float calculateVoltageDiff(const CircuitElement& circuitElement) {
    switch (circuitElement.state)
    {
    case DrawState::RESISTOR:
    case DrawState::VOLTAGE_SOURCE:
    case DrawState::CURRENT_SOURCE:
        return abs(circuitElement.startNode->value - circuitElement.endNode->value);
    default:
        return 0.0f;
    }
}

bool SolveCircuit(std::list<NodeObject>& nodes, std::list<CircuitElement>& circuitElements) {
    int* nodeCounter{};
    bool groundExists{ false };
    
    std::list<Graph::Node*> tempGrounds;


    for (auto& node : nodes) {
        if (node.graphNode.visited)
            continue;

        Graph::Node* lastNode;

        bool foundGround = false;

        nodeCounter = Graph::dfs(node.graphNode, lastNode, foundGround);

        if (!foundGround && lastNode) {
            lastNode->isGround = true;
            (*nodeCounter)--;
            tempGrounds.push_back(lastNode);
        }
    }

    if (nodeCounter)
        *nodeCounter = 0;

    Circuit circuit;

    for (auto& circuitElement : circuitElements) {
        CircuitNode nodei{ 
            circuitElement.startNode->graphNode.value, 
            circuitElement.startNode->graphNode.isGround
        };
        CircuitNode nodej{ 
            circuitElement.endNode->graphNode.value, 
            circuitElement.endNode->graphNode.isGround
        };

        switch (circuitElement.state)
        {
        case DrawState::RESISTOR:
            std::cout << "R" << " " << nodei.index << ", " << nodej.index << ", GROUNDS: " << nodei.isGround << ", " << nodej.isGround << '\n';
            circuit.addElement(std::make_unique<Resistor>(nodei, nodej, circuitElement.value));
            break;

        case DrawState::VOLTAGE_SOURCE:
            std::cout << "V" << " " << nodei.index << ", " << nodej.index << ", GROUNDS: " << nodei.isGround << ", " << nodej.isGround << '\n';
            circuit.addElement(std::make_unique<VoltageSource>(nodej, nodei, circuitElement.value));
            break;

        case DrawState::CURRENT_SOURCE:
            std::cout << "C" << " " << nodei.index << ", " << nodej.index << ", GROUNDS: " << nodei.isGround << ", " << nodej.isGround << '\n';
            circuit.addElement(std::make_unique<CurrentSource>(nodei, nodej, circuitElement.value));
            break;

        case DrawState::WIRE:
            std::cout << "W" << " " << nodei.index << ", " << nodej.index << ", GROUNDS: " << nodei.isGround << ", " << nodej.isGround << '\n';
            circuit.addElement(std::make_unique<ShortCircuit>(nodej, nodei));
            break;

        default:
            break;
        }
    }

    auto solution = circuit.solve();
    auto values = circuit.makeUnkownValuePair(solution);

    std::cout << "Solution: ";
    for (auto& pair : values) {
        std::cout << pair.first << " = " << pair.second << ", ";
    }
    std::cout << '\n';


    for (auto& node : nodes) {
        node.graphNode.visited = false;


        if (solution.size() < 1) {
            node.solved = false;
            continue;
        }
        
        node.solved = true;

        if (node.graphNode.isGround) {
            node.value = 0.0f;
            continue;
        }

        if (solution.size() > node.graphNode.value) {
            node.value = static_cast<float>(solution[node.graphNode.value]);
        }
    }

    int extractedVoltageCurrents = 0;
    int unkownNodes = circuit.getUnkownsNumber();

    for (auto& circuitElement : circuitElements) {
        if (solution.size() < 1) {
            circuitElement.current = 0.0f;
            continue;
        }

        std::size_t solutionIndex;

        switch (circuitElement.state)
        {
        case DrawState::RESISTOR:
            circuitElement.current = calculateVoltageDiff(circuitElement) / circuitElement.value;
            break;

        case DrawState::WIRE:
        case DrawState::VOLTAGE_SOURCE:
            solutionIndex = static_cast<std::size_t>(unkownNodes + extractedVoltageCurrents);

            if (solution.size() > solutionIndex) {
                circuitElement.current = solution[solutionIndex];
                ++extractedVoltageCurrents;
            }

            break;

        case DrawState::CURRENT_SOURCE:
            circuitElement.current = circuitElement.value;
            break;
        }
    }

    for (auto* tempGround : tempGrounds) {
        tempGround->isGround = false;
    }

    return solution.size() >= 1;
}