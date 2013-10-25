#pragma once
#include "Message/Message.h"

class VelocityChangeMessage : public Message
{
public:
  VelocityChangeMessage(double x, double y) : m_dx(x), m_dy(y) { }
  MessageType GetType() const override { return MessageType::VELOCITY_CHANGE; }

  double GetXChange() const { return m_dx; }
  double GetYChange() const { return m_dy; }

private:
  const double m_dx;
  const double m_dy;
};
