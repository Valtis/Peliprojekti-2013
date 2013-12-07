#pragma once
#include "Message/Message.h"

class PlaySoundEffectMessage : public Message
{
public:
  PlaySoundEffectMessage(int effectID) : m_effectId(effectID) { }
  MessageType GetType() const override { return MessageType::PLAY_SOUND_EFFECT; }
  int GetEffectID() const { return m_effectId; }
private:
  const int m_effectId;
};