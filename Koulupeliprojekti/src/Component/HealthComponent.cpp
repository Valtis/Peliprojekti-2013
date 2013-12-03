#include "Component/HealthComponent.h"
#include "Message/CollisionMessage.h"
#include "Message/MessageFactory.h"
#include "Component/FactionComponent.h"
#include "Entity/Entity.h"
#include "Utility/LoggerManager.h"

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
  GetOwner()->RegisterMessageHandler(MessageType::TAKE_DAMAGE, Priority::NORMAL, [&](Message *msg) 
  {
    this->TakeDamage();
    return MessageHandling::STOP_HANDLING;
  }
  );
}

void HealthComponent::TakeDamage()
{
  --m_hitpoints;
  if (m_hitpoints <= 0)
  {
    if (m_lives <= 0)
    {
      auto deathMsg = MessageFactory::CreateTerminateEntityMessage(GetOwner());

      GetOwner()->SendMessage(deathMsg.get());
      auto faction = static_cast<FactionComponent *>(GetOwner()->GetComponent(ComponentType::FACTION));
      if (faction != nullptr && faction->GetFaction() == Faction::ENEMY) 
      {
        auto deathSoundMsg = MessageFactory::CreatePlaySoundEffectMessage(MONSTER_DEATH);
        GetOwner()->SendMessage(deathSoundMsg.get());
      }
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
