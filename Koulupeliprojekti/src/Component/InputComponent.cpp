#include "Component/InputComponent.h"
#include "Message/Commands/ControlCommand.h"
#include "Entity/Entity.h"
#include "Message/MessageFactory.h"
#include "Message/SetVelocityMessage.h"
#include "Component/LocationComponent.h"
#include "UI/InputManager.h"
#include <SDL.h>
InputComponent::InputComponent() : m_lastFireTick(0), m_id(0)
{

}

InputComponent::InputComponent(int id) : m_id(id), m_lastFireTick(0)
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
  manager.RegisterInputHandler([&](Command *cmd) { return this->HandleInput(cmd); } , 5);
}


// this is kinda terrible, needs to be refactored

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

  Direction xDir = Direction::UNCHANGED;
  Direction yDir = Direction::UNCHANGED;

  // TODO: clean up this
  switch (command->GetCommand())
  {
  case Action::LEFT:
    xDir = command->GetState() == KeyState::DOWN ? Direction::LEFT : Direction::NONE;
    break;
  case Action::RIGHT:
    xDir = command->GetState() == KeyState::DOWN ? Direction::RIGHT : Direction::NONE;
    break;

  case Action::UP:
    yDir = command->GetState() == KeyState::DOWN ? Direction::UP : Direction::NONE;

    break;
  case Action::DOWN:
    yDir = command->GetState() == KeyState::DOWN ? Direction::DOWN : Direction::NONE;
    break;
  case Action::JUMP:
    if (command->GetState() == KeyState::DOWN)
      Jump();
    return MessageHandling::STOP_HANDLING;
  case Action::FIRE:
    Fire();
    MessageHandling::STOP_HANDLING;
  default:
    return MessageHandling::PASS_FORWARD; 
    break;
  }


  auto velMsg = MessageFactory::CreateSetVelocityMessage(xDir, yDir);
  GetOwner()->SendMessage(velMsg.get());
  return MessageHandling::PASS_FORWARD;
}

void InputComponent::Jump()
{
  auto loc = static_cast<LocationComponent*>(GetOwner()->GetComponent(ComponentType::LOCATION));
  if (loc == nullptr)
    return;
  if (!loc->CanIJump())
    return;

  auto message = MessageFactory::CreateSetVelocityMessage(Direction::UNCHANGED, Direction::UP);
  GetOwner()->SendMessage(message.get());
}

void InputComponent::Fire()
{
  if (m_lastFireTick + 500 >= SDL_GetTicks())
  {
    return;
  }

  m_lastFireTick = SDL_GetTicks();

  auto msg = MessageFactory::CreateSpawnEntityMessage(EntityType::BULLET, GetOwner());
  GetOwner()->SendMessage(msg.get());
}
