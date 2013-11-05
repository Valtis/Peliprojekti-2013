#pragma once
#include "Message/Message.h"
enum class Velocity : int { X, Y, XY };

class SetVelocityMessage : public Message
{
public:
  SetVelocityMessage(double x, double y, Velocity dir) : m_xVelocity(x), m_yVelocity(y), m_direction(dir) { }
  MessageType GetType() const override { return MessageType::SET_VELOCITY; }

  Velocity GetDirection() const { return m_direction; }
  double GetXVelocity() const { return m_xVelocity; }
  double GetYVelocity() const { return m_yVelocity; }

private:
  const double m_xVelocity;
  const double m_yVelocity;
  const Velocity m_direction;
};
