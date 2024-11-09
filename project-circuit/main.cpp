#include <iostream>
#include "AugmentedMatrix.h"
#include "MatrixSolve.h"
#include <string_view>
#include <string>
#include <set>
#include <iomanip>

using Number = double;

enum CircuitType {
    resistor,
    currentSource,
    voltageSource
};

class CircuitStamp {
public:
    CircuitStamp(int nodeI, int nodeJ, Number value, CircuitType type)
        : m_nodeI(nodeI)
        , m_nodeJ(nodeJ)
        , m_value(value)
        , m_type(type)
    {}

    virtual void contribute(AugmentedMatrix<Number>& a, int ground) = 0;

    int getNodeI() const { return m_nodeI; };
    int getNodeJ() const { return m_nodeJ; };

    CircuitType getType() const { return m_type; }

protected:
    int m_nodeI{};
    int m_nodeJ{};
    Number m_value{};
    CircuitType m_type;
};

int getRealNode(int node, int ground) {
    if (node <= ground)
        return node;

    return node - 1;
}


class Resistor : public CircuitStamp {
public:
    Resistor(int nodeI, int nodeJ, Number value)
        : CircuitStamp(nodeI, nodeJ, value, resistor)
    {}

    void contribute(AugmentedMatrix<Number>& a, int ground) override {
        if (m_nodeI != ground && m_nodeJ != ground) {
            int workingNodeI = getRealNode(m_nodeI, ground);
            int workingNodeJ = getRealNode(m_nodeJ, ground);

            a.get(workingNodeI, workingNodeI) += 1 / m_value;
            a.get(workingNodeJ, workingNodeJ) += 1 / m_value;
            a.get(workingNodeI, workingNodeJ) += -1 / m_value;
            a.get(workingNodeJ, workingNodeI) += -1 / m_value;

            return;
        }
        
        int workingNode = getRealNode(m_nodeI != ground ? m_nodeI : m_nodeJ, ground);

        a.get(workingNode, workingNode) += 1 / m_value;
    };
};

class CurrentSource : public CircuitStamp {
public:
    CurrentSource(int nodeI, int nodeJ, Number value)
        : CircuitStamp(nodeI, nodeJ, value, currentSource)
    {}

    void contribute(AugmentedMatrix<Number>& a, int ground) override {
        if (m_nodeI != ground && m_nodeJ != ground) {
            int workingNodeI = getRealNode(m_nodeI, ground);
            int workingNodeJ = getRealNode(m_nodeJ, ground);

            a.get(workingNodeJ, a.columns() - 1) += m_value;
            a.get(workingNodeI, a.columns() - 1) += -m_value;

            return;
        }

        
        int workingNode = getRealNode(m_nodeI != ground ? m_nodeI : m_nodeJ, ground);

        a.get(workingNode, a.columns() - 1) += (m_nodeJ != ground ? 1 : -1) * m_value;
    }
};

class VoltageSource : public CircuitStamp {
public:
    VoltageSource(int nodeI, int nodeJ, Number value)
        : CircuitStamp(nodeI, nodeJ, value, voltageSource)
    {}

    void contribute(AugmentedMatrix<Number>& a, int ground) override {
        a.get(m_voltageRow, a.columns() - 1) =  m_value;

        if (m_nodeI != ground && m_nodeJ != ground) {
            int workingNodeI = getRealNode(m_nodeI, ground);
            int workingNodeJ = getRealNode(m_nodeJ, ground);

            a.get(m_voltageRow, workingNodeI) = 1;
            a.get(m_voltageRow, workingNodeJ) = -1;

            a.get(workingNodeI, m_voltageRow) = 1;
            a.get(workingNodeJ, m_voltageRow) = -1;

            a.get(m_voltageRow, a.columns() - 1) = m_value;

            return;
        }

        int workingNode = getRealNode(m_nodeI != ground ? m_nodeI : m_nodeJ, ground);

        a.get(m_voltageRow, workingNode) = 1;
        a.get(workingNode, m_voltageRow) = 1;
    }

    void setVoltageRow(int value) { m_voltageRow = value; }

private:
    int m_voltageRow{};
};


class Circuit {
public:
    using AutoElement = std::unique_ptr<CircuitStamp>;
    using UnkownValuePair = std::vector<std::pair<std::string, Number>>;
    Circuit() = default;

    void addElement(std::unique_ptr<CircuitStamp> ce) {
        nodes.insert(ce.get()->getNodeI());
        nodes.insert(ce.get()->getNodeJ());

        if (ce.get()->getType() == voltageSource)
            ++m_voltageSourcesCount;

        circuitElements.push_back(std::move(ce));
    }

    std::vector<Number> solve() {
        const int unkownNodes = static_cast<int>(nodes.size()) - 1;
        const int voltageStartingRow = unkownNodes;
        AugmentedMatrix<Number> solvingMatrix(unkownNodes + m_voltageSourcesCount);

        int currentVoltageSource = 0;

        for (const auto& element : circuitElements) {
            if (element.get()->getType() == voltageSource) {
                VoltageSource* v = dynamic_cast<VoltageSource*>(element.get());

                if (!v) {
                    --m_voltageSourcesCount;
                    continue;
                }

                v->setVoltageRow(voltageStartingRow + currentVoltageSource);
                ++currentVoltageSource;
            }

            element.get()->contribute(solvingMatrix, m_ground);
        }


        return Matrix::solve(solvingMatrix);
    }

    UnkownValuePair makeUnkownValuePair(const std::vector<Number>& v) {
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

    void setGround(int value) { m_ground = value; }
private:
    int m_ground{0};
    int m_voltageSourcesCount{ 0 };
    std::vector<AutoElement> circuitElements;
    std::set<int> nodes;

};



int main()
{
    Circuit circuit;

    circuit.addElement(std::make_unique<VoltageSource>(1, 0, 56));
    circuit.addElement(std::make_unique<Resistor>(1, 2, 2000));
    circuit.addElement(std::make_unique<Resistor>(2, 3, 6000));
    circuit.addElement(std::make_unique<Resistor>(2, 4, 6000));
    circuit.addElement(std::make_unique<Resistor>(4, 3, 2000));
    circuit.addElement(std::make_unique<Resistor>(3, 0, 4000));
    circuit.addElement(std::make_unique<Resistor>(4, 0, 4000));


    circuit.setGround(0);

    auto solution{ circuit.makeUnkownValuePair(circuit.solve()) };

   for (const auto& pair : solution) {
        std::cout << pair.first << "=" << pair.second << ", ";
    }

    
}