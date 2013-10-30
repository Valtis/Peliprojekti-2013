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
  void OnAttatchingToEntity() override;
  SDL_Rect GetHitbox(); // World coordinates

private:
  bool HandleCollisionMessage(Message *msg);
  void Collide(Entity *entity, int sides);
  SDL_Rect m_hitbox; // Local coordinates
};