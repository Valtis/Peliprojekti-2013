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
    return false;
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
      auto msg = MessageFactory::CreateTerminateEntityMessage(GetOwner());
      GetOwner()->SendMessage(msg.get());
    }
    else 
    {
      --m_lives;
      m_hitpoints = m_maxHitpoints;
      // send location reset message
    }
  }
}
