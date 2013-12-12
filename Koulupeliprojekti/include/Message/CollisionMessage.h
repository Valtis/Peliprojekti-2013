#pragma once
#include "Message/Message.h"
#include <vector>
#include <SDL.h>

enum class HitboxType;
class Entity;
#include "Utility/DirectionEnum.h"

struct CollisionHit
{
  SDL_Rect isect;
  SDL_Point point;
  Direction v_side;
  Direction h_side;
  HitboxType hit_type;
  std::vector<Entity *> entities;
};

class CollisionMessage : public Message
{
public:
  CollisionMessage(CollisionHit hit) : m_hit(hit) { }
  ~CollisionMessage() { }

  MessageType GetType() const override { return MessageType::COLLISION; }

  std::vector<Entity *> GetEntities() const { return m_hit.entities; }
  Direction GetHorizontalSide() const { return m_hit.h_side; }
  Direction GetVerticalSide() const { return m_hit.v_side; }
  SDL_Point GetPoint() const { return m_hit.point; }
  SDL_Rect GetIntersection() const { return m_hit.isect; }
  HitboxType GetHitType() const { return m_hit.hit_type; }

private:
  const CollisionHit m_hit;
};
