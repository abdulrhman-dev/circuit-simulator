#pragma once
#include <array>
#include "raylib.h"
#include "UIConstants.h"
#include "Input.h"
#include "StateHelper.h"
#include "CircuitCalculations.h"
#include "StatusText.h"

namespace Keyboard {
	void handleModeSwitch(DrawState& currentDrawState);

	void handleUndo(std::list<CircuitElement>& circuitElements, std::list<NodeObject>& nodes, Input& input, bool isCurrentElementDrawing);

	void handleBeginInsertMode(std::list<CircuitElement>& circuitElements, Input& input);

	void handleSolve(std::list<CircuitElement>& circuitElements, std::list<NodeObject>& nodes, StatusText& statusText);
}