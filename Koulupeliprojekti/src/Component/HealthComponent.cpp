#include "Component/HealthComponent.h"
#include "Message/MessageFactory.h"
#include "Message/AddHealthMessage.h"
#include "Message/QueryFactionMessage.h"
#include "Utility/LoggerManager.h"
#include "Entity/Entity.h"

HealthComponent::HealthComponent() : m_hitpoints(0), m_lives(0)
{

}

HealthComponent::HealthComponent(int hitpoints, int maxHitpoints, int lives) : m_hitpoints(hitpoints), m_maxHitpoints(maxHitpoints), m_lives(lives)
{

}

HealthComponent::~HealthComponent()
{

}

void HealthComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::TAKE_DAMAGE, Priority::LOWEST, [=](Message *msg) 
  {
    this->TakeDamage();
    return MessageHandling::STOP_HANDLING;
  }
  );

  GetOwner()->RegisterMessageHandler(MessageType::ADD_HEALTH, Priority::LOWEST, 
    [=](Message *msg) { return this->HandleAddHealthMessage(msg); });
}

void HealthComponent::TakeDamage()
{
  auto queryFacMsg = MessageFactory::CreateQueryFactionMessage();
  Faction faction = Faction::NONE;
  
  if ( GetOwner()->SendMessage(queryFacMsg.get()))
  {
    faction = queryFacMsg->GetFaction();
  }

  --m_hitpoints;
  
  if (m_hitpoints > 0)
  {
     auto hitMsg = MessageFactory::CreatePlaySoundMessage(SoundEffectType::TAKE_DAMAGE);
     GetOwner()->SendMessage(hitMsg.get());
  }
  
  if (m_hitpoints == 0)
  {
    if (m_lives == 0)
    {
      auto deathMsg = MessageFactory::CreateTerminateEntityMessage(GetOwner());

      GetOwner()->SendMessage(deathMsg.get());
      auto deathSoundMsg = MessageFactory::CreatePlaySoundMessage(SoundEffectType::DEATH);
      GetOwner()->SendMessage(deathSoundMsg.get());
      
    }
    else 
    {
      --m_lives;
      m_hitpoints = m_maxHitpoints;
      auto msg = MessageFactory::CreateResetEntityPositionMessage(GetOwner());
      GetOwner()->SendMessage(msg.get());
    }
  }
}

MessageHandling HealthComponent::HandleAddHealthMessage(Message *msg)
{
  auto healthMsg = static_cast<AddHealthMessage *>(msg);
  m_hitpoints = std::min(m_maxHitpoints, m_hitpoints + healthMsg->GetHealth());
  return MessageHandling::STOP_HANDLING;
}