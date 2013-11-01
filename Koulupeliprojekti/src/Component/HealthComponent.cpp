#include "Component/HealthComponent.h"
#include "Entity/Entity.h"

HealthComponent::HealthComponent() : m_hitpoints(0), m_lives(0)
{

}

HealthComponent::HealthComponent(int hitpoints, int lives) : m_hitpoints(hitpoints), m_lives(lives)
{

}

HealthComponent::~HealthComponent()
{

}

void HealthComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::COLLISION,  Priority::NORMAL, 
    [&](Message *msg){ return this->HandleCollisionMessage(msg); });
}

bool HealthComponent::HandleCollisionMessage(Message *msg)
{

  --m_hitpoints;
  if (m_hitpoints < 0)
  {
    if (m_lives <= 0)
    {
      // send termination message
    }
    else 
    {
      --m_lives;
      m_hitpoints = m_maxHitpoints;
    }
  
    return false;
}
