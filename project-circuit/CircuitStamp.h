#ifndef CIRCUIT_STAMP_H
#define CIRCUIT_STAMP_H
#include "AugmentedMatrix.h"
using Number = double;

struct CircuitNode {
    int index{};
    bool isGround{};
};

class CircuitStamp {
public:
    enum CircuitType {
        resistor,
        currentSource,
        voltageSource,
        shortCircuit
    };

    CircuitStamp(CircuitNode nodeI, CircuitNode nodeJ, Number value, CircuitType type)
        : m_nodeI(nodeI)
        , m_nodeJ(nodeJ)
        , m_value(value)
        , m_type(type)
    {}

    virtual void contribute(AugmentedMatrix<Number>& a) = 0;

    CircuitNode getNodeI() const { return m_nodeI; };
    CircuitNode getNodeJ() const { return m_nodeJ; };

    CircuitType getType() const { return m_type; }

protected:
    CircuitNode m_nodeI{};
    CircuitNode m_nodeJ{};
    Number m_value{};
    CircuitType m_type;
};

class Resistor : public CircuitStamp {
public:
    Resistor(CircuitNode nodeI, CircuitNode nodeJ, Number value)
        : CircuitStamp(nodeI, nodeJ, value, resistor)
    {}

    void contribute(AugmentedMatrix<Number>& a) override;
};

class CurrentSource : public CircuitStamp {
public:
    CurrentSource(CircuitNode nodeI, CircuitNode nodeJ, Number value)
        : CircuitStamp(nodeI, nodeJ, value, currentSource)
    {}

    void contribute(AugmentedMatrix<Number>& a) override;
};

class VoltageSource : public CircuitStamp {
public:
    VoltageSource(CircuitNode nodeI, CircuitNode nodeJ, Number value)
        : CircuitStamp(nodeI, nodeJ, value, voltageSource)
    {}

    void contribute(AugmentedMatrix<Number>& a) override;

    void setVoltageRow(int value) { m_voltageRow = value; }

protected:
    VoltageSource(CircuitNode nodeI, CircuitNode nodeJ, Number value, CircuitType type)
        : CircuitStamp(nodeI, nodeJ, value, type)
    {}

private:
    int m_voltageRow{};
};


class ShortCircuit : public VoltageSource {
public:
    ShortCircuit(CircuitNode nodeI, CircuitNode nodeJ)
        : VoltageSource(nodeI, nodeJ, 0, shortCircuit)
    {}

};

#endif // !CIRCUIT_STAMP_H
