#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <string>
#include <set>
#include <memory>
#include "CircuitStamp.h"
#include "MatrixSolve.h"

class Circuit {
public:
    using AutoElement = std::unique_ptr<CircuitStamp>;
    using UnkownValuePair = std::vector<std::pair<std::string, Number>>;

    Circuit() = default;

    void addElement(std::unique_ptr<CircuitStamp> ce);

    std::vector<Number> solve();

    UnkownValuePair makeUnkownValuePair(const std::vector<Number>& v);

    void setGround(int value) { m_ground = value; }
private:
    int m_ground{ 0 };
    int m_voltageSourcesCount{ 0 };
    std::vector<AutoElement> circuitElements;
    std::set<int> nodes;

};
#endif // !CIRCUIT_H
