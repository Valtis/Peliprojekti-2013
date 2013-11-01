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
  GetOwner()->RegisterMessageHandler(MessageType::COLLISION,  Priority::HIGHEST, 
    [&](Message *msg){ return this->HandleCollisionMessage(msg); });
}

bool HealthComponent::HandleCollisionMessage(Message *msg)
{
  if (msg->GetType() != MessageType::COLLISION)
  {
    LoggerManager::GetLog(COMPONENT_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in HealthComponent::HandleCollisionMessage() - ignoring");
    return true;
  }


  auto colMsg = static_cast<CollisionMessage *> (msg);
  auto myFaction = static_cast<FactionComponent *>(GetOwner()->GetComponent(ComponentType::FACTION));
  auto otherFaction = static_cast<FactionComponent *>(colMsg->GetEntity()->GetComponent(ComponentType::FACTION));

  if (myFaction != nullptr && otherFaction != nullptr && myFaction->GetFaction() == otherFaction->GetFaction())
  {
    return true;
  }


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
    }
  }
  
  return true;
}
