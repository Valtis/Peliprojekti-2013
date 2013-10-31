#pragma once
#include "Component/Component.h"
#include <SDL.h>

class CollisionComponent : public Component
{
public:
  CollisionComponent();
  CollisionComponent(int w, int h);
  CollisionComponent(int x, int y, int w, int h);
  CollisionComponent(SDL_Rect &box) : m_hitbox(box) { };
  ~CollisionComponent();
  SDL_Rect GetHitbox(); // World coordinates

private:
  SDL_Rect m_hitbox; // Local coordinates
};