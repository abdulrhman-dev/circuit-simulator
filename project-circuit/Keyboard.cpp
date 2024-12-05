#include "Keyboard.h"
#include "raymath.h"

void Keyboard::handleCameraActions(Camera2D& camera) {
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f / camera.zoom);
        camera.target = Vector2Add(camera.target, delta);
    }


    float wheel = GetMouseWheelMove();

    if (wheel == 0) {
        if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_EQUAL))
            wheel = 2.5f;
        else if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_MINUS))
            wheel = -2.5f;
    }

    if (camera.zoom >= 1.5 && wheel > 0)
        return;


    if (wheel != 0)
    {
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

        camera.offset = GetMousePosition();

        camera.target = mouseWorldPos;

        float scaleFactor = 1.0f + (0.25f * fabsf(wheel));
        if (wheel < 0) scaleFactor = 1.0f / scaleFactor;
        camera.zoom = Clamp(camera.zoom * scaleFactor, 0.125f, 64.0f);
    }
  
}

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