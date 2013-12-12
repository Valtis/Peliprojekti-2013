#include "Component/InputComponent.h"
#include "Message/Commands/ControlCommand.h"
#include "Message/MessageFactory.h"
#include "Message/SetVelocityMessage.h"
#include "Message/QueryCanIJumpMessage.h"
#include "UI/InputManager.h"
#include "Entity/Entity.h"
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
  GetOwner()->RegisterMessageHandler(MessageType::CONTROL_COMMAND, Priority::LOWEST,
    [=](Message *msg) { return this->HandleInput(static_cast<Command*>(msg)); });
}

void InputComponent::RegisterInputHandler(InputManager &manager)
{
  manager.RegisterInputHandler([&](Command *cmd) { return this->HandleInput(cmd); } , 5);
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

  Direction moveXDirection = Direction::UNCHANGED;
  Direction moveYDirection = Direction::UNCHANGED;
  Direction fireDirection = Direction::UNCHANGED;

  // TODO: clean up this
  switch (command->GetCommand())
  {
  case Action::LEFT:
    moveXDirection = command->GetState() == KeyState::DOWN ? Direction::LEFT : Direction::NONE;
    fireDirection = moveXDirection;
    break;
  case Action::RIGHT:
    moveXDirection = command->GetState() == KeyState::DOWN ? Direction::RIGHT : Direction::NONE;
    fireDirection = moveXDirection;
    break;

  case Action::UP:
    moveYDirection = command->GetState() == KeyState::DOWN ? Direction::UP : Direction::NONE;
    break;
  case Action::DOWN:
     moveYDirection = command->GetState() == KeyState::DOWN ? Direction::DOWN : Direction::NONE;
    break;

  case Action::AIM_UP:

    fireDirection = command->GetState() == KeyState::DOWN ? Direction::UP : Direction::NONE;
    
    break;
  case Action::AIM_DOWN:
    fireDirection = command->GetState() == KeyState::DOWN ? Direction::DOWN : Direction::NONE;
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


  auto velMsg = MessageFactory::CreateSetVelocityMessage(moveXDirection, moveYDirection);
  auto fireMsg = MessageFactory::CreateSetFireDirectionMessage(fireDirection);
  
  GetOwner()->SendMessage(velMsg.get());
  GetOwner()->SendMessage(fireMsg.get());
  return MessageHandling::PASS_FORWARD;
}

void InputComponent::Jump()
{

  auto queryJumpMsg = MessageFactory::CreateQueryJumpMessage();  
  if (!GetOwner()->SendMessage(queryJumpMsg.get()) || !queryJumpMsg->CanIJump())
  {
    return;
  }

  auto message = MessageFactory::CreateSetVelocityMessage(Direction::UNCHANGED, Direction::UP);
  GetOwner()->SendMessage(message.get());
  auto jmpMsg = MessageFactory::CreatePlaySoundMessage(SoundEffectType::JUMP);
  GetOwner()->SendMessage(jmpMsg.get());

}

void InputComponent::Fire()
{
  if (m_lastFireTick + 500 >= SDL_GetTicks())
  {
    return;
  }

  m_lastFireTick = SDL_GetTicks();

  auto spawnMsg = MessageFactory::CreateSpawnEntityMessage(EntityType::BULLET, GetOwner());
  auto soundMsg = MessageFactory::CreatePlaySoundMessage(SoundEffectType::SHOOT);
  GetOwner()->SendMessage(spawnMsg.get());
  GetOwner()->SendMessage(soundMsg.get());
}
