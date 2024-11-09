#include <iostream>
#include <iomanip>
#include "Circuit.h"

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