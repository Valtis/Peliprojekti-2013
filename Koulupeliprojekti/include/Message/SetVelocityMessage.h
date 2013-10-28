#pragma once
#include "Message/Message.h"

class SetVelocityMessage : public Message
{
public:
  SetVelocityMessage(double x, double y) : m_xVelocity(x), m_yVelocity(y) { }
  MessageType GetType() const override { return MessageType::SET_VELOCITY; }

  double GetXVelocity() const { return m_xVelocity; }
  double GetYVelocity() const { return m_yVelocity; }

private:
  const double m_xVelocity;
  const double m_yVelocity;
};
