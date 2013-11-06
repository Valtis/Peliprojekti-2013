#include "Component/PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
  : m_acceleration_x(0.0), m_acceleration_y(1.5), m_terminal_velocity(6.0)
{

}

PhysicsComponent::PhysicsComponent(double terminal_velocity)
  : m_acceleration_x(0.0), m_acceleration_y(1.5),
    m_terminal_velocity(terminal_velocity)
{

}

PhysicsComponent::PhysicsComponent(double acceleration_x, double acceleration_y)
  : m_terminal_velocity(6.0)
{
  SetAcceleration(acceleration_x, acceleration_y);
}

PhysicsComponent::PhysicsComponent(double acceleration_x, double acceleration_y,
                                   double terminal_velocity)
  : m_terminal_velocity(terminal_velocity)
{
  SetAcceleration(acceleration_x, acceleration_y);
}

PhysicsComponent::~PhysicsComponent()
{

}

double PhysicsComponent::GetAccelerationX()
{
  return m_acceleration_x;
}

double PhysicsComponent::GetAccelerationY()
{
  return m_acceleration_y;
}

double PhysicsComponent::GetTerminalVelocity()
{
  return m_terminal_velocity;
}

void PhysicsComponent::SetAcceleration(double x, double y)
{
  m_acceleration_x = x;
  m_acceleration_y = y;
}
