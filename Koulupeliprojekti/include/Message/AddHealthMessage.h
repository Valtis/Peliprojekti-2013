#pragma once
#include "Message/Message.h"

class AddHealthMessage : public Message
{
public:
  AddHealthMessage(int health) : m_health(health) { }
  ~AddHealthMessage() { }

  MessageType GetType() const override { return MessageType::ADD_HEALTH; }
  int GetHealth() const { return m_health; }
private:
  const int m_health;
};