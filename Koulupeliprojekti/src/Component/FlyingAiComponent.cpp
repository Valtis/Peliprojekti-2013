#include <SDL.h>
#include <cmath>
#include <memory>

#include "Message/CollisionMessage.h"
#include "Message/QueryLocationMessage.h"
#include "Message/MessageFactory.h"
#include "Message/Commands/ControlCommand.h"
#include "Entity/Entity.h"
#include "Component/FlyingAiComponent.h"

FlyingAiComponent::FlyingAiComponent(Entity* player) : AiComponent(), m_player(player), m_attacking(false), m_attack_tick(0), m_following(true), m_hit_a_wall(false), m_collision_tick(0)
{

}

void FlyingAiComponent::Update(double ticksPassed)
{
  m_attack_tick += ticksPassed;

  auto playerLocationQuery = MessageFactory::CreateQueryLocationMessage();
  auto ownLocationQuery =  MessageFactory::CreateQueryLocationMessage();

  if (!GetOwner()->SendMessage(ownLocationQuery.get()) || !m_player->SendMessage(playerLocationQuery.get()))
  {
    return;
  }

  double player_x = playerLocationQuery->GetX();
  double player_y = playerLocationQuery->GetY();

  double x = ownLocationQuery->GetX();
  double y = ownLocationQuery->GetY();

  double distance = sqrt(pow(player_x - x,2) + pow(player_y - y,2));
  if (distance > 260)
    m_following = true;


  if (m_following)
  {
    m_player_x = player_x;
    m_player_y = player_y;
    Follow(x,y);
    if (SDL_fabs(player_x - x) < 220 && player_y - y < 240)
    {
      if (m_attack_tick > 150)
      {
        m_attack_tick = 0;
        m_attacking = true;
        m_following = false;
        wiggle = 50;
      }
    }
  }
  else
  {
    if (m_attacking)
    {
      Attack(x,y);
      wiggle -= static_cast<int>(ticksPassed / 100.0);
    }
  }
}

MessageHandling FlyingAiComponent::HandleCollisionMessage(Message *msg)
{

  if (msg->GetType() != MessageType::COLLISION)
    return MessageHandling::STOP_HANDLING;
  if (m_attack_tick - m_collision_tick < 10)
	m_hit_a_wall = true;
  else
	m_hit_a_wall = false;

  CollisionMessage* collision = static_cast<CollisionMessage*>(msg);
  Direction side = collision->GetVerticalSide();

  Action action = Action::UP;
  if (side == Direction::UP)
	action = Action::DOWN;
  side = collision->GetHorizontalSide();
  if (side == Direction::LEFT)
	action = Action::RIGHT;
  if (side == Direction::RIGHT)
	action = Action::LEFT;

  if (action == Action::UP || action == Action::DOWN)
  	m_attacking = false;
  else
	m_collision_tick = m_attack_tick;

  std::unique_ptr<ControlCommand> command(new ControlCommand(action, KeyState::DOWN,-1));
  GetOwner()->SendMessage(command.get());

  return MessageHandling::PASS_FORWARD;
}

void FlyingAiComponent::Attack(double x, double y)
{
  std::unique_ptr<ControlCommand> command;

  if (SDL_fabs(m_player_x - x) < 20 && SDL_fabs(m_player_y - y) < 20)
  {
    command.reset(new ControlCommand(Action::UP, KeyState::DOWN,-1));
    GetOwner()->SendMessage(command.get());
    m_attacking = false;
    return;
  }

  if (x > m_player_x + wiggle)
  {
    command.reset(new ControlCommand(Action::LEFT,KeyState::DOWN,-1));
    GetOwner()->SendMessage(command.get());
  }
  else if (x < m_player_x - wiggle)
  {
    command.reset(new ControlCommand(Action::RIGHT,KeyState::DOWN,-1));
    GetOwner()->SendMessage(command.get());
  }
  command.reset(new ControlCommand(Action::DOWN,KeyState::DOWN,-1));

  GetOwner()->SendMessage(command.get());

}

void FlyingAiComponent::Follow(double x, double y)
{
  std::unique_ptr<ControlCommand> command;
  if (x > m_player_x + 200)
  {
    command.reset(new ControlCommand(Action::LEFT,KeyState::DOWN,-1));
    GetOwner()->SendMessage(command.get());
  }
  else if (x < m_player_x - 50)
  {
    command.reset(new ControlCommand(Action::RIGHT,KeyState::DOWN,-1));
    GetOwner()->SendMessage(command.get());
  }
  if (m_hit_a_wall)
	return;
  if (m_player_y - y < 200)
  {
    command.reset(new ControlCommand(Action::UP,KeyState::DOWN,-1));
    GetOwner()->SendMessage(command.get());
  }
  else if (m_player_y - y > 320)
  {
    command.reset(new ControlCommand(Action::DOWN,KeyState::DOWN,-1));
    GetOwner()->SendMessage(command.get());
  }
}
