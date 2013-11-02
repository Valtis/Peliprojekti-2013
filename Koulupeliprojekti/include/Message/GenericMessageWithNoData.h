#pragma  once
#include "Message/Message.h"

class GenericMessageWithNoData : public Message
{
public:
  GenericMessageWithNoData(MessageType type) : m_type(type) { }
  MessageType GetType() const override { return m_type; }
private:
  const MessageType m_type;
};