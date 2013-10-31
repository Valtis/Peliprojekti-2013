#pragma once
#include "Component/Component.h"
#include <SDL.h>

class PhysicsComponent : public Component
{
public:
  PhysicsComponent();
  PhysicsComponent(SDL_Point &direction);
  PhysicsComponent(float acceleration, float terminal_velocity);
  PhysicsComponent(SDL_Point &direction,
                   float acceleration, float terminal_velocity);
  ~PhysicsComponent();
  SDL_Point GetDirection();
  float GetAcceleration();
  float GetTerminalVelocity();

private:
  SDL_Point m_direction;
  float m_acceleration;
  float m_terminal_velocity;
};