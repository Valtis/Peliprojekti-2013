#pragma once
#include "Component/Component.h"
#include "Sound/SoundEffectDefs.h"
#include <map>
class SoundComponent : public Component
{
public:
  SoundComponent() { }
  ~SoundComponent() { }
  void AddSoundEffect(SoundEffectType type, int id)
  {
    m_effectIDs[type] = id;
  }
protected:
  void OnAttatchingToEntity() override;

private:
  MessageHandling HandlePlaySoundEffectMessage(Message *msg);
  std::map<SoundEffectType, int> m_effectIDs;
};