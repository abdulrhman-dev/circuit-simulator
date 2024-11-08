#include <iostream>
#include "AugmentedMatrix.h"
#include "MatrixSolve.h"
#include <set>

using Number = double;

enum CircuitType {
    resistor,
    currentSource,
    voltageSource,
    shortCircuit
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
                if (!v)
                    continue;
                v->setVoltageRow(voltageStartingRow + currentVoltageSource);
                ++currentVoltageSource;
            }

            element.get()->contribute(solvingMatrix, m_ground);
        }


        return Matrix::solve(solvingMatrix);
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

    circuit.addElement(std::unique_ptr<CircuitStamp>(new Resistor(1, 0, 2)));
    circuit.addElement(std::unique_ptr<CircuitStamp>(new Resistor(1, 0, 6)));
    circuit.addElement(std::unique_ptr<CircuitStamp>(new Resistor(2, 0, 3)));
    circuit.addElement(std::unique_ptr<CircuitStamp>(new Resistor(2, 3, 4)));
    circuit.addElement(std::unique_ptr<CircuitStamp>(new VoltageSource(3, 0, 14)));
    circuit.addElement(std::unique_ptr<CircuitStamp>(new VoltageSource(1, 2, 6)));

    std::vector<Number> solution{ circuit.solve() };

    for (Number value : solution) {
        std::cout << value << " ";
    }
}

