#pragma once
#include "CircuitElement.h"
#include "NodeObject.h"
#include "raylib.h"
#include "string"
#include "UIConstants.h"
#include "TextHelper.h"
#include "optional"
using CircuitElementIterator = std::list<CircuitElement>::iterator;

class Input {
private:

	CircuitElement* inputCircuitElement{ nullptr };
	CircuitElementIterator inputElementIterator{};

	std::list<CircuitElement>& m_circuitElements;

	std::string value{};
	bool inputMode{ false };
	bool periodExists{ false };
	bool firstTime{ true };
	std::optional<MetricPrefix> prefix;
	Font m_font;
public:
	Input(const Font& font, std::list<CircuitElement>& circuitElements)
		: m_font(font)
		, m_circuitElements(circuitElements)
	{}

	void assign(CircuitElementIterator circuitElement);

	void reset();

	int isInputMode() const { return inputMode; }

	void handle();

	void draw();

	void handleBackSpace();

	void handleCycle();


};

