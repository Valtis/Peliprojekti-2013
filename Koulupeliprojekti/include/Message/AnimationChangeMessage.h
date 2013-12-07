#pragma once
#include "Message/Message.h"

class AnimationChangeMessage : public Message
{
public:
  AnimationChangeMessage(int id) : m_id(id) { }
  ~AnimationChangeMessage() { }

  int GetId() const { return m_id; }
  MessageType GetType() const override { return MessageType::CHANGE_ANIMATION_MESSAGE; }

private:
  const int m_id;

};