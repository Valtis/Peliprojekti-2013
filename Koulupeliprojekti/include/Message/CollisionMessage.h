#pragma once
#include "Message/Message.h"

enum class CollisionSide : int { NONE, UP, DOWN, LEFT, RIGHT };

class CollisionMessage : public Message
{
public:
  CollisionMessage(Entity *entity,SDL_Rect &intersection,CollisionSide side)
    : m_entity(entity), m_intersection(intersection), m_side(side) { }
  MessageType GetType() const override { return MessageType::COLLISION; }

  SDL_Rect GetIntersection() const { return m_intersection; }
  Entity * GetEntity() const { return m_entity; }
  CollisionSide GetSide() const { return m_side; }

private:
  CollisionSide m_side;
  SDL_Rect m_intersection;
  Entity *m_entity;
};
