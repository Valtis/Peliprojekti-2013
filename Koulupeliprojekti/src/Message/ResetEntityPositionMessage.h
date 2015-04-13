#pragma once
#include "Message/Message.h"
class Entity;
class ResetEntityPositionMessage : public Message 
{
public:
  ResetEntityPositionMessage(Entity *entity) : m_entity(entity) { }
  ~ResetEntityPositionMessage() { }

  MessageType GetType() const override { return MessageType::RESET_ENTITY_POSITION; }
  Entity *GetEntity() const { return m_entity; }

private:
  Entity *m_entity;
};