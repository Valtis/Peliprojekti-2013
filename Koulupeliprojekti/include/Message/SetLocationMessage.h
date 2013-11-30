#pragma once
#include "Message/Message.h"

class SetLocationMessage : public Message
{

public:
  SetLocationMessage(int x, int y) : m_x(x), m_y(y) { }
  ~SetLocationMessage() { }

  MessageType GetType() const override { MessageType::SET_LOCATION; }
  const double GetX() const { return m_x; }
  const double GetY() const { return m_y; }

private:
  const double m_x;
  const double m_y;
};