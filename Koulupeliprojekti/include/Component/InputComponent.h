#pragma once

#include "Component/Component.h"
class Command;
class InputComponent : public Component
{
public:
	InputComponent();
	~InputComponent();

	bool HandleInput(Command *command);

private:

};