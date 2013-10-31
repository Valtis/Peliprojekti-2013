#include "Component/PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
  : m_acceleration(1.0f), m_terminal_velocity(4.0f)
{
  m_direction.x = 0;
  m_direction.y = 1;
}

PhysicsComponent::PhysicsComponent(SDL_Point &direction)
  : m_acceleration(1.0f), m_terminal_velocity(4.0f)
{
  m_direction.x = direction.x;
  m_direction.y = direction.y;
}

PhysicsComponent::PhysicsComponent(float acceleration, float terminal_velocity)
{
  m_acceleration = acceleration;
  m_terminal_velocity = terminal_velocity;
  m_direction.x = 0;
  m_direction.y = 1;
}

PhysicsComponent::PhysicsComponent(SDL_Point &direction,
                                   float acceleration, float terminal_velocity)
{
  m_acceleration = acceleration;
  m_terminal_velocity = terminal_velocity;
  m_direction.x = direction.x;
  m_direction.y = direction.y;
}

PhysicsComponent::~PhysicsComponent()
{

}

SDL_Point PhysicsComponent::GetDirection()
{
  return m_direction;
}

float PhysicsComponent::GetAcceleration()
{
  return m_acceleration;
}

float PhysicsComponent::GetTerminalVelocity()
{
  return m_terminal_velocity;
}
