#include "Component/InputComponent.h"
#include "Message/Commands/ControlCommand.h"
#include "Entity/Entity.h"
#include "Message/MessageFactory.h"
#include "UI/InputManager.h"
InputComponent::InputComponent()
{

}

InputComponent::~InputComponent()
{

}

void InputComponent::OnAttatchingToEntity()
{

}

void InputComponent::RegisterInputHandler(InputManager &manager)
{
  manager.RegisterInputHandler([&](Command *cmd) { return this->HandleInput(cmd); }, 5);
}

bool InputComponent::HandleInput( Command *msg )
{
  if (msg->GetType() != MessageType::CONTROL_COMMAND)
  {
    return false;
  }

  auto *command = static_cast<ControlCommand *>(msg);
  double xVel = 2;
  double yVel = 2;
  if (command->GetState() == false)
  {
    xVel = yVel = 0;
  }

  // kinda ugly, probably needs to be refactored
  double newXVelocity = 0;
  double newYVelocity = 0;
  // TODO: clean up this
  switch (command->GetCommand())
  {
  case Action::LEFT:
    newXVelocity = -xVel;

    break;
  case Action::RIGHT:
    newXVelocity = xVel;
    break;

  case Action::UP:
    newYVelocity = -yVel;
    break;
  case Action::DOWN:
    newYVelocity = yVel;
    break;
  default:
    return false; // not handling anything
    break;
  }



  auto message = MessageFactory::CreateSetVelocityMessage(newXVelocity, newYVelocity);
  GetOwner()->SendMessage(message.get());

  return true;
}
