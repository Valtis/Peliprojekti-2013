#pragma once
#include <vector>
#include "Component/Component.h"
#include <SDL.h>

enum class HitboxType : int { SOLID, TRIGGER };

class CollisionComponent : public Component
{
public:
  CollisionComponent();
  ~CollisionComponent();
  void AddHitbox(SDL_Rect &box, const HitboxType type);
  void AddHitbox(int x, int y, int w, int h, const HitboxType type);
  const std::vector<SDL_Rect> GetHitboxes(const HitboxType type); // World coordinates
  
private:
  SDL_Rect TransformHitbox(const SDL_Rect box); // Local -> World

  // Local coordinates
  std::vector<SDL_Rect> m_object_hitboxes;
  std::vector<SDL_Rect> m_wall_hitboxes;
  std::vector<SDL_Rect> m_trigger_hitboxes;
};