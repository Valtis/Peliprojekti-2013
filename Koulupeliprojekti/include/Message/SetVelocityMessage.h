#pragma once
#include "Message/Message.h"
#include "Utility/DirectionEnum.h"
class SetVelocityMessage : public Message
{
public:
  SetVelocityMessage(Direction x, Direction y) : m_xDirection(x), m_yDirection(y) { }
  MessageType GetType() const override { return MessageType::SET_VELOCITY; }

  Direction GetXDirection() const { return m_xDirection; }
  Direction GetYDirection() const { return m_yDirection; }

private:
  const Direction m_xDirection;
  const Direction m_yDirection;
};
