#include "Component/InputComponent.h"
#include "Message/Commands/ControlCommand.h"
InputComponent::InputComponent()
{

}

InputComponent::~InputComponent()
{

}

bool InputComponent::HandleInput(Command *command)
{
	if (command->GetType() != MessageType::CONTROL_COMMAND)
	{
		return true;
	}



	return false;
}
