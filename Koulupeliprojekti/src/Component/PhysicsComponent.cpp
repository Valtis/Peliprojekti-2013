#include "Component/PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
  : m_acceleration_x(0.0f), m_acceleration_y(1.0f), m_terminal_velocity(4.0f)
{

}

PhysicsComponent::PhysicsComponent(float terminal_velocity)
  : m_acceleration_x(0.0f), m_acceleration_y(1.0f),
    m_terminal_velocity(terminal_velocity)
{

}

PhysicsComponent::PhysicsComponent(float acceleration_x, float acceleration_y)
  : m_terminal_velocity(4.0f)
{
  SetAcceleration(acceleration_x, acceleration_y);
}

PhysicsComponent::PhysicsComponent(float acceleration_x, float acceleration_y,
                                   float terminal_velocity)
  : m_terminal_velocity(terminal_velocity)
{
  SetAcceleration(acceleration_x, acceleration_y);
}

PhysicsComponent::~PhysicsComponent()
{

}

float PhysicsComponent::GetAccelerationX()
{
  return m_acceleration_x;
}

float PhysicsComponent::GetAccelerationY()
{
  return m_acceleration_y;
}

float PhysicsComponent::GetTerminalVelocity()
{
  return m_terminal_velocity;
}

void PhysicsComponent::SetAcceleration(float x, float y)
{
  if (x + y <= 1.0f && x + y >= -1.0f)
  {
    m_acceleration_x = x;
    m_acceleration_y = y;
  }
  else
  {
    double _d, _x, _y;
    _x = static_cast<double>(x);
    _y = static_cast<double>(y);
    _d = _x + _y;

    m_acceleration_x = static_cast<float>(_x / _d);
    m_acceleration_y = static_cast<float>(_y / _d);
  }
}
