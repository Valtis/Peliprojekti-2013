#include "Component/InputComponent.h"
#include "Message/Commands/KeyboardCommand.h"
InputComponent::InputComponent()
{

}

InputComponent::~InputComponent()
{

}

bool InputComponent::HandleInput(Command *command)
{
	if (command->GetType() != MessageType::KEYBOARD_COMMAND)
	{
		return true;
	}



	return false;
}