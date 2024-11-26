#ifndef INPUT_H
#define INPUT_H
#include "Output.h"
#include "raylib.h"
#include "string"
class Input {
private:
	CircuitElement* inputCircuitElement{ nullptr };
	std::string input{};
	bool inputMode{ false };
	bool periodExists{ false };
public:
	void assign(CircuitElement* input) {
		if (input) {
			inputMode = true;
			inputCircuitElement = input;
		}
	}

	void reset() {
		input = "";
		inputCircuitElement = nullptr;
		inputMode = false;
		periodExists = false;
	}

	void handle() {
		if (!inputCircuitElement)
			return;

		int key = GetCharPressed();

		while (key > 0)
		{
			if ((key >= 48) && (key <= 57) || (key == 46 && !periodExists))
			{
				if (key == 46) periodExists = true;

				input += static_cast<char>(key);
			}

			key = GetCharPressed();
		}

		if (IsKeyPressed(KEY_BACKSPACE) && input.size() > 0)
		{
			if (input.back() == '.') periodExists = false;

			input.pop_back();
		}

		if (inputCircuitElement && input != "")
			inputCircuitElement->value = std::stof(input);
		else if (inputCircuitElement && input == "")
			inputCircuitElement->value = 0.0f;

		if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
			reset();
	}
};

// m (1e-3,1000e-3)
// mirco 1e-6,1000e-6)
// nano 1e-

#endif