#include "Component/InputComponent.h"
#include "Message/Commands/ControlCommand.h"
#include "Entity/Entity.h"
#include "Message/MessageFactory.h"
#include "Message/SetVelocityMessage.h"
#include "Component/LocationComponent.h"
#include "UI/InputManager.h"
#include <SDL.h>
InputComponent::InputComponent() : m_debugLastFireTick(0), m_id(0)
{

}

InputComponent::InputComponent(int id) : m_id(id), m_debugLastFireTick(0)
{

}

InputComponent::~InputComponent()
{

}

void InputComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::CONTROL_COMMAND, Priority::NORMAL,
    [&](Message *msg) { return this->HandleInput(static_cast<Command*>(msg)); });
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

  // check if AI connected and got a non-AI command
  if (m_id == -1 && command->GetController() != -1)
	return false;

  double xVel = 2;
  double yVel = 2;
  if (command->GetState() == false)
  {
    xVel = yVel = 0;
  }

  // kinda ugly, probably needs to be refactored
  double newXVelocity = 0;
  double newYVelocity = 0;
  Velocity dir = Velocity::X;
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
    dir = Velocity::Y;
    break;
  case Action::DOWN:
    newYVelocity = yVel;
    dir = Velocity::Y;
    break;
  case Action::JUMP:
    if (command->GetState() == true)
    	Jump();
    return true;
  case Action::FIRE:
    Fire();
    break;
  default:
    return false; // not handling anything
    break;
  }

  if (dir == Velocity::X)
  {
    auto message = MessageFactory::CreateSetVelocityMessage(newXVelocity, Velocity::X);
    GetOwner()->SendMessage(message.get());
  }
  else
  {
    auto message = MessageFactory::CreateSetVelocityMessage(newYVelocity, Velocity::Y);
    GetOwner()->SendMessage(message.get());
  }


  return true;
}

void InputComponent::Jump()
{
  auto loc = static_cast<LocationComponent*>(GetOwner()->GetComponent(ComponentType::LOCATION));
  if (loc == nullptr)
	return;
  if (!loc->CanIJump())
	return;

  auto message = MessageFactory::CreateSetVelocityMessage(-12, Velocity::Y);
  GetOwner()->SendMessage(message.get());
}

void InputComponent::Fire()
{
  if (m_debugLastFireTick + 500 >= SDL_GetTicks())
  {
    return;
  }
 
  m_debugLastFireTick = SDL_GetTicks();

  auto msg = MessageFactory::CreateSpawnEntityMessage(EntityType::BULLET, GetOwner());
  GetOwner()->SendMessage(msg.get());
}
