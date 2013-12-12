#include "Component/SoundComponent.h"
#include "Entity/Entity.h"
#include "Message/PlaySoundEffectMessage.h"
#include "Message/MessageFactory.h"
#include "Entity/Entity.h"
void SoundComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::PLAY_SOUND_EFFECT, Priority::LOWEST,
    [=](Message *msg) { return this->HandlePlaySoundEffectMessage(msg); });
}

MessageHandling SoundComponent::HandlePlaySoundEffectMessage(Message *msg)
{
  auto soundMsg  = static_cast<PlaySoundEffectMessage *>(msg);
  if (m_effectIDs.count(soundMsg->GetSoundType()) != 0) 
  {
    auto soundMngrMessage = 
      MessageFactory::CreateSoundManagerPlaySoundMessage(
      m_effectIDs[soundMsg->GetSoundType()]);
    GetOwner()->SendMessage(soundMngrMessage.get());
  }
  return MessageHandling::STOP_HANDLING;
}