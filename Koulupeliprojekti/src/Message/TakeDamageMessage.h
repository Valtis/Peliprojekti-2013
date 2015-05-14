#pragma once
#include "Message/Message.h"

class TakeDamageMessage : public Message
{
public:
  TakeDamageMessage(int amount) : m_amount(amount) { }
  ~TakeDamageMessage() { }

  MessageType GetType() const override { return MessageType::TAKE_DAMAGE; }
  int GetAmount() const { return m_amount; }
private:
  const int m_amount;
};