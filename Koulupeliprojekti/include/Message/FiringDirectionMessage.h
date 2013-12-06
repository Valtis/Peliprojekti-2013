#pragma once
#include "Message/Message.h"
#include "Utility/DirectionEnum.h"
class FiringDirectionMessage : public Message
{
public:
  FiringDirectionMessage(Direction direction) : m_direction(direction) { }
  ~FiringDirectionMessage() { }
  Direction GetDirection() { return m_direction; }
  MessageType GetType() const override { return MessageType::FIRE_DIRECTION; }

private:

  const Direction m_direction;
};