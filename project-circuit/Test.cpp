#include "CircuitStamp.h"

#include "Circuit.h"



int main() {
	Circuit circuit;

	circuit.addElement(std::make_unique<Resistor>(CircuitNode { 1 }, CircuitNode{ 5 }, 5));
	circuit.addElement(std::make_unique<Resistor>(CircuitNode{ 1 }, CircuitNode{ 2 }, 5));
	circuit.addElement(std::make_unique<Resistor>(CircuitNode{ 2 }, CircuitNode{ 3 }, 5));

	circuit.addElement(std::make_unique<VoltageSource>(CircuitNode{ 1 }, CircuitNode{ 0 }, 5));

	circuit.addElement(std::make_unique<ShortCircuit>(CircuitNode{ 0 }, CircuitNode{ 0, true }));
	circuit.addElement(std::make_unique<ShortCircuit>(CircuitNode{ 2 }, CircuitNode{ 4 }));
	circuit.addElement(std::make_unique<ShortCircuit>(CircuitNode{ 0 }, CircuitNode{ 3 }));


	circuit.addElement(std::make_unique<CurrentSource>(CircuitNode{ 4 }, CircuitNode{ 5 }, 5));


	auto solution = circuit.solve();
	for (auto& entry : circuit.makeUnkownValuePair(solution)) {
		std::cout << entry.first << "= " << entry.second << ", ";
	}
	



	return 0;
}