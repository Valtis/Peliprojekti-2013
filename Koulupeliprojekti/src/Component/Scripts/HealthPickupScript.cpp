#include "Component/Scripts/HealthPickupScript.h"
#include "Entity/Entity.h"
#include "Message/MessageFactory.h"
#include "Message/CollisionMessage.h"
#include "Component/CollisionComponent.h"
#include "Component/FactionComponent.h"

HealthPickupScript::HealthPickupScript(int health) : m_healthOnPickup(health)
{

}

HealthPickupScript::~HealthPickupScript()
{

}


void HealthPickupScript::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::COLLISION, Priority::HIGHEST, 
    [&](Message *msg) { return this->HandleCollisionMessage(msg); });
}

MessageHandling HealthPickupScript::HandleCollisionMessage(Message *msg)
{
  
  auto colMsg = static_cast<CollisionMessage *>(msg);
  if (colMsg->GetHitType() == HitboxType::TRIGGER)
  {
    return MessageHandling::STOP_HANDLING;
  }

  auto healthMsg = MessageFactory::CreateAddHealthMessage(m_healthOnPickup);
  bool terminate = false;

  // any entity with faction component can pick it up. Includes bullets and enemies
  // bullets have max 1hp so effectively they just destroy it
  for (auto e : colMsg->GetEntities())
  {


    if (e->GetComponent(ComponentType::FACTION) && 
      static_cast<FactionComponent *>(e->GetComponent(ComponentType::FACTION))->GetFaction() == Faction::PLAYER)
    {
      e->SendMessage(healthMsg.get());
      terminate = true;
    }
  }

  if (terminate)
  {
    auto terminateMsg = MessageFactory::CreateTerminateEntityMessage(GetOwner());
    GetOwner()->SendMessage(terminateMsg.get());
    return MessageHandling::PASS_FORWARD;
  }

  return MessageHandling::PASS_FORWARD;
}