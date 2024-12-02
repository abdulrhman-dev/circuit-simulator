#include "CircuitCalculations.h"
#include "Circuit.h"
#include "Output.h"
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
    int nodeCounter{ 0 };
    int ground{ 0 };

    for (auto& node : nodes) {
        if (node.graphNode.visited)
            continue;

        Graph::dfs(node.graphNode, nodeCounter);
        ++nodeCounter;
    }

    Circuit circuit;

    for (auto& circuitElement : circuitElements) {
        int nodei = circuitElement.startNode->graphNode.value;
        int nodej = circuitElement.endNode->graphNode.value;

        switch (circuitElement.state)
        {
        case DrawState::RESISTOR:
            circuit.addElement(std::make_unique<Resistor>(nodei, nodej, circuitElement.value));
            break;
        case DrawState::VOLTAGE_SOURCE:
            circuit.addElement(std::make_unique<VoltageSource>(nodej, nodei, circuitElement.value));
            break;
        case DrawState::CURRENT_SOURCE:
            circuit.addElement(std::make_unique<CurrentSource>(nodei, nodej, circuitElement.value));
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

    if (solution.size() <= 0)
        return false;



    for (auto& node : nodes) {
        node.graphNode.visited = false;
        node.solved = true;

        if (node.graphNode.value == ground) {
            node.value = 0.0f;
            continue;
        }

        if (solution.size() > getRealNode(node.graphNode.value, ground)) {
            node.value = static_cast<float>(solution[getRealNode(node.graphNode.value, ground)]);
        }
    }

    int extractedVoltageCurrents = 0;
    int unkownNodes = circuit.getUnkownsNumber();

    for (auto& circuitElement : circuitElements) {
        std::size_t solutionIndex;

        switch (circuitElement.state)
        {
        case DrawState::RESISTOR:
            circuitElement.current = calculateVoltageDiff(circuitElement) / circuitElement.value;
            break;

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

    return true;
}