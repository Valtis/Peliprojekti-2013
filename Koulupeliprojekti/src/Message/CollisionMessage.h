#pragma once
#include "Message/Message.h"
#include <vector>
enum class HitboxType;

enum class CollisionSide : int { NONE, UP, DOWN, LEFT, RIGHT };

struct CollisionHit
{
  SDL_Rect isect;
  SDL_Point point;
  CollisionSide v_side;
  CollisionSide h_side;
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
  CollisionSide GetHorizontalSide() const { return m_hit.h_side; }
  CollisionSide GetVerticalSide() const { return m_hit.v_side; }
  SDL_Point GetPoint() const { return m_hit.point; }
  SDL_Rect GetIntersection() const { return m_hit.isect; }
  HitboxType GetHitType() const { return m_hit.hit_type; }

private:
  const CollisionHit m_hit;
};
