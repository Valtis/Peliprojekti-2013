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
  manager.RegisterInputHandler([&](Command *cmd) { MessageHandling h = this->HandleInput(cmd); return (h == MessageHandling::PASS_FORWARD); }, 5);
}

MessageHandling InputComponent::HandleInput( Command *msg )
{
  if (msg->GetType() != MessageType::CONTROL_COMMAND)
  {
    return MessageHandling::PASS_FORWARD;
  }

  auto *command = static_cast<ControlCommand *>(msg);

  // check if AI connected and got a non-AI command
  if (m_id == -1 && command->GetController() != -1)
    return MessageHandling::PASS_FORWARD;

  double xVel = 3;
  double yVel = 3;
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
    return MessageHandling::STOP_HANDLING;
  case Action::FIRE:
    Fire();
    MessageHandling::STOP_HANDLING;
  default:
    return MessageHandling::PASS_FORWARD; 
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


  return MessageHandling::PASS_FORWARD;
}

void InputComponent::Jump()
{
  auto loc = static_cast<LocationComponent*>(GetOwner()->GetComponent(ComponentType::LOCATION));
  if (loc == nullptr)
    return;
  if (!loc->CanIJump())
    return;

  auto message = MessageFactory::CreateSetVelocityMessage(-13, Velocity::Y);
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
