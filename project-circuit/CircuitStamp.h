#ifndef CIRCUIT_STAMP_H
#define CIRCUIT_STAMP_H
#include "AugmentedMatrix.h"
using Number = double;

class CircuitStamp {
public:
    enum CircuitType {
        resistor,
        currentSource,
        voltageSource,
        shortCircuit
    };

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

class Resistor : public CircuitStamp {
public:
    Resistor(int nodeI, int nodeJ, Number value)
        : CircuitStamp(nodeI, nodeJ, value, resistor)
    {}

    void contribute(AugmentedMatrix<Number>& a, int ground) override;
};

class CurrentSource : public CircuitStamp {
public:
    CurrentSource(int nodeI, int nodeJ, Number value)
        : CircuitStamp(nodeI, nodeJ, value, currentSource)
    {}

    void contribute(AugmentedMatrix<Number>& a, int ground) override;
};

class VoltageSource : public CircuitStamp {
public:
    VoltageSource(int nodeI, int nodeJ, Number value)
        : CircuitStamp(nodeI, nodeJ, value, voltageSource)
    {}

    void contribute(AugmentedMatrix<Number>& a, int ground) override;

    void setVoltageRow(int value) { m_voltageRow = value; }

private:
    int m_voltageRow{};
};


int getRealNode(int node, int ground);
#endif // !CIRCUIT_STAMP_H
