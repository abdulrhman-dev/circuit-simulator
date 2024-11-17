#include "Circuit.h"

void Circuit::addElement(std::unique_ptr<CircuitStamp> ce) {
    nodes.insert(ce.get()->getNodeI());
    nodes.insert(ce.get()->getNodeJ());

    if (ce.get()->getType() == CircuitStamp::voltageSource)
        ++m_voltageSourcesCount;

    circuitElements.push_back(std::move(ce));
}

std::vector<Number> Circuit::solve() {
    const int unkownNodes = getUnkownsNumber();
    AugmentedMatrix<Number> solvingMatrix(unkownNodes + m_voltageSourcesCount);

    int currentVoltageSource = 0;

    for (const auto& element : circuitElements) {
        if (element.get()->getType() == CircuitStamp::voltageSource) {
            VoltageSource* v = dynamic_cast<VoltageSource*>(element.get());

            if (!v) {
                --m_voltageSourcesCount;
                continue;
            }

            v->setVoltageRow(unkownNodes + currentVoltageSource);
            ++currentVoltageSource;
        }

        element.get()->contribute(solvingMatrix, m_ground);
    }


    std::cout << "MATRIX: \n";

    MatrixSolver::print(solvingMatrix);

    std::cout << '\n';


    return MatrixSolver::solve(solvingMatrix);
}

Circuit::UnkownValuePair Circuit::makeUnkownValuePair(const std::vector<Number>& v) {
    UnkownValuePair result;

    if (nodes.size() <= 1 || v.size() == 0)
        return result;

    std::size_t totalCount = nodes.size() + static_cast<std::size_t>(m_voltageSourcesCount) - 1;
    result.reserve(totalCount);


    for (int node : nodes) {
        if (node == m_ground)
            continue;

        int workingNode = getRealNode(node, m_ground);
        result.emplace_back("V_" + std::to_string(node), v[workingNode]);
    }

    int voltageCount{};
    for (std::size_t i = nodes.size() - 1; i < totalCount; ++i) {
        result.emplace_back("IE_" + std::to_string(voltageCount), v[i]);
        ++voltageCount;
    }

    return result;
}