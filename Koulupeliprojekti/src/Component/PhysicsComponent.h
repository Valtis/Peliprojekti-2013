#pragma once
#include "Component/Component.h"
#include <SDL.h>

class PhysicsComponent : public Component
{
public:
  PhysicsComponent();
  PhysicsComponent(double terminal_velocity);
  PhysicsComponent(double acceleration_x, double acceleration_y);
  PhysicsComponent(double acceleration_x, double acceleration_y, double terminal_velocity);
  ~PhysicsComponent();
  double GetAccelerationX();
  double GetAccelerationY();
  double GetTerminalVelocity();
  void SetAcceleration(double x, double y);

private:
  double m_acceleration_x;
  double m_acceleration_y;
  double m_terminal_velocity;
};