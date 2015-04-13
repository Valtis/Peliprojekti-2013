#pragma once
#include "Message/Message.h"

class StartBlinkingMessage : public Message
{
public:
  StartBlinkingMessage(double duration) : m_blinkDuration(duration) { }
  ~StartBlinkingMessage() { }

  MessageType GetType() const override { return MessageType::START_BLINKING; }
  double GetBlinkDuration() const { return m_blinkDuration; }
private:
  const double m_blinkDuration;
};