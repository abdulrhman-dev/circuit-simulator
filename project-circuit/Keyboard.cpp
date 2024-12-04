#include "Keyboard.h"

void Keyboard::handleModeSwitch(DrawState& currentDrawState) {
	if (IsKeyPressed(KEY_R)) currentDrawState = DrawState::RESISTOR;
	else if (IsKeyPressed(KEY_V)) currentDrawState = DrawState::VOLTAGE_SOURCE;
	else if (IsKeyPressed(KEY_C)) currentDrawState = DrawState::CURRENT_SOURCE;
	else if (IsKeyPressed(KEY_W)) currentDrawState = DrawState::WIRE;
	else if (IsKeyPressed(KEY_G)) currentDrawState = DrawState::GROUND;
}

void Keyboard::handleUndo(std::list<CircuitElement>& circuitElements, std::list<NodeObject>& nodes, Input& input, bool isCurrentElementDrawing) {
	bool canBeginInsertMode = !isCurrentElementDrawing && !input.isInputMode() && circuitElements.size() > 0;

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z) && canBeginInsertMode)
		deleteElement(std::prev(circuitElements.end()), circuitElements, nodes);
}

void Keyboard::handleBeginInsertMode(std::list<CircuitElement>& circuitElements, Input& input) {
	if (IsKeyPressed(KEY_TAB) && circuitElements.size() >= 1) 
		input.assign(circuitElements.begin());
}

void Keyboard::handleSolve(std::list<CircuitElement>& circuitElements, std::list<NodeObject>& nodes, StatusText& statusText) {
	statusText.reset();
	statusText.clearText();

	static std::optional<bool> solved{};

	if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_ENTER)) {
		solved = SolveCircuit(nodes, circuitElements);
	}

	if (solved.has_value() && !solved.value()) {
		statusText.addText("The drawn circuit cannot be solved.");
	}

}