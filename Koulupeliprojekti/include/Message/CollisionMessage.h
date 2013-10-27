#pragma once
#include "Message/Message.h"

class CollisionMessage : public Message
{
public:
  CollisionMessage(Entity *entity,SDL_Rect &intersection) : m_entity(entity), m_intersection(intersection) { }
  MessageType GetType() const override { return MessageType::COLLISION; }

  SDL_Rect GetIntersection() const { return m_intersection; }
  Entity * GetEntity() const { return m_entity; }

private:
  SDL_Rect m_intersection;
  Entity *m_entity;
};
