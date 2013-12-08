#pragma once
#include "Message/Message.h"

class SoundManagerPlaySoundMessage : public Message
{
public:
  SoundManagerPlaySoundMessage(int id ) : m_effectID(id) { }
  MessageType GetType() const override { return MessageType::SOUND_MANAGER_PLAY_SOUND; }
  int GetEffectID() const { return m_effectID; }
private:
  const int m_effectID;
};