#pragma once
#include "Message/Message.h"
#include "Utility/FactionEnum.h"

class QueryCanIJumpMessage : public Message
{
public:
  QueryCanIJumpMessage() { }
  ~QueryCanIJumpMessage() { }

  bool CanIJump() const { return m_canIJump; }
  void SetJump(bool j) { m_canIJump = j; }

  MessageType GetType() const override { return MessageType::QUERY_JUMP; }

private:
  bool m_canIJump;
};