#pragma once
#include "Component/Component.h"
#include <SDL.h>

class CollisionComponent : public Component
{
public:
  CollisionComponent();
  CollisionComponent(int w, int h);
  CollisionComponent(int w, int h, int x, int y);
  CollisionComponent(SDL_Rect &box) : m_hitbox(box) { };
  ~CollisionComponent();
  void OnAttatchingToEntity() override;
  void Update(double ticksPassed) override;
  void HandleCollision(Entity *entity, SDL_Rect &intersection);
  SDL_Rect GetHitbox(); // World coordinates

private:
  bool HandleCollisionMessage(Message *msg);
  void Collide(Entity *entity, int sides);
  SDL_Rect m_hitbox; // Local coordinates
};