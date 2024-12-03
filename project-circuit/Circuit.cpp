#include "Circuit.h"

void Circuit::addElement(std::unique_ptr<CircuitStamp> ce) {
    if(!ce.get()->getNodeI().isGround)
        nodes.insert(ce.get()->getNodeI().index);
    if (!ce.get()->getNodeJ().isGround)
        nodes.insert(ce.get()->getNodeJ().index);

    if (ce.get()->getType() == CircuitStamp::voltageSource || ce.get()->getType() == CircuitStamp::shortCircuit)
        ++m_voltageSourcesCount;

    circuitElements.push_back(std::move(ce));
}

std::vector<Number> Circuit::solve() {
    const int unkownNodes = getUnkownsNumber();
    AugmentedMatrix<Number> solvingMatrix(unkownNodes + m_voltageSourcesCount);

    int currentVoltageSource = 0;

    for (const auto& element : circuitElements) {
        if (element.get()->getType() == CircuitStamp::voltageSource || element.get()->getType() == CircuitStamp::shortCircuit) {
            VoltageSource* v = dynamic_cast<VoltageSource*>(element.get());

            if (!v) {
                --m_voltageSourcesCount;
                continue;
            }

            v->setVoltageRow(unkownNodes + currentVoltageSource);
            ++currentVoltageSource;
        }

        element.get()->contribute(solvingMatrix);
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

    std::size_t totalCount = getUnkownsNumber() + static_cast<std::size_t>(m_voltageSourcesCount);
    result.reserve(totalCount);


    for (int node : nodes) {
        result.emplace_back("V_" + std::to_string(node), v[node]);
    }

    int voltageCount{};
    for (std::size_t i = nodes.size(); i < totalCount; ++i) {
        result.emplace_back("IE_" + std::to_string(voltageCount), v[i]);
        ++voltageCount;
    }

    return result;
}