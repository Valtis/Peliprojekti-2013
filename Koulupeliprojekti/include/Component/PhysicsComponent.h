#pragma once
#include "Component/Component.h"
#include <SDL.h>

class PhysicsComponent : public Component
{
public:
  PhysicsComponent();
  PhysicsComponent(float terminal_velocity);
  PhysicsComponent(float acceleration_x, float acceleration_y);
  PhysicsComponent(float acceleration_x, float acceleration_y, float terminal_velocity);
  ~PhysicsComponent();
  float GetAccelerationX(); // Always between 0.0 - 1.0
  float GetAccelerationY(); // Always between 0.0 - 1.0
  float GetTerminalVelocity();
  // Ensures that x and y are always between 0.0 - 1.0
  void SetAcceleration(float x, float y);

private:
  float m_acceleration_x;
  float m_acceleration_y;
  float m_terminal_velocity;
};