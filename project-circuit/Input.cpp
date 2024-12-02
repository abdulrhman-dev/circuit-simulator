#include "Input.h"

void Input::assign(CircuitElementIterator circuitElement) {
	if (circuitElement != m_circuitElements.end()) {
		inputElementIterator = circuitElement;
		inputMode = true;
		inputCircuitElement = &(*circuitElement);
	}
}

void Input::reset() {
	value = "";
	inputCircuitElement = nullptr;
	inputMode = false;
	periodExists = false;
	prefix = std::nullopt;
}


void Input::handle() {
	if (!inputCircuitElement)
		return;

	inputCircuitElement->setOpacity(0.6f);

	int key = GetCharPressed();

	while (key > 0)
	{
		bool isNumber = (key >= 48) && (key <= 57);
		bool canInsertPeriod = key == 46 && !periodExists && value != "";
		bool isPrefix = value != "" && metricPrefixes.find(key) != metricPrefixes.end();

		if (prefix == std::nullopt && (isNumber || canInsertPeriod || isPrefix))
		{
			value += static_cast<char>(key);
		}

		if (canInsertPeriod)
			periodExists = true;

		if (isPrefix) 
			prefix = MetricPrefix{ key , metricPrefixes[key]};

		

		key = GetCharPressed();
	}

	if (IsKeyPressed(KEY_BACKSPACE) && value.size() > 0)
	{
		if (value.back() == '.') periodExists = false;
		if (prefix != std::nullopt) prefix = std::nullopt;
		value.pop_back();
	}

	if (inputCircuitElement) {
		if (value != "") {
			inputCircuitElement->value = std::stof(value);
			if (prefix) inputCircuitElement->value *= prefix.value().second;
		} else if (value == "")
			inputCircuitElement->value = 0.0f;
	}

	handleCycle();

	if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
		reset();
}

void Input::handleCycle() {

	if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_TAB)) {
		reset();

		if (inputElementIterator == m_circuitElements.begin())
			return;

		auto prevElement = std::prev(inputElementIterator);

		while (
			prevElement->state == DrawState::WIRE || prevElement->state == DrawState::GROUND
			)
		{
			if (prevElement == m_circuitElements.begin()) return;

			prevElement = std::prev(prevElement);
		}

		assign(prevElement);
	}

	if (!IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_TAB)) {
		reset();

		auto nextElement = std::next(inputElementIterator);


		if (nextElement == m_circuitElements.end())
			return;


		while (nextElement->state == DrawState::WIRE || nextElement->state == DrawState::GROUND)
		{
			nextElement = std::next(nextElement);

			if (nextElement == m_circuitElements.end()) return;
		}

		if (nextElement != m_circuitElements.end()) {
			assign(nextElement);
		}
	}

}

void Input::draw() {
	if (!inputMode || !inputCircuitElement)
		return;
	Rectangle rec{ UI::cellSize - 10.0f,UI::height - UI::cellSize - 20, UI::width - 2 * (UI::cellSize - 10), 40 };
	DrawRectangleLinesEx(rec, 2.0f, UI::WIRE_COLOR);

	std::string text;

	if (value != "")
		text = value;
	else
		text = toString(inputCircuitElement->value);

	text += getUnit(inputCircuitElement->state);

	DrawTextEx(m_font, text.c_str(), Vector2{ UI::cellSize,  UI::height - UI::cellSize - 10 }, 20, 1, BLACK);
}


