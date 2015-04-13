#pragma once
#include "Message/Message.h"
#include "Sound/SoundEffectDefs.h"
class PlaySoundEffectMessage : public Message
{
public:
  PlaySoundEffectMessage(SoundEffectType type) : m_type(type) { }
  MessageType GetType() const override { return MessageType::PLAY_SOUND_EFFECT; }
  SoundEffectType GetSoundType() const { return m_type; }
private:
  const SoundEffectType m_type;
};