#include "Component/Scripts/EndLevelScriptComponent.h"
#include "Component/FactionComponent.h"
#include "Message/CollisionMessage.h"
#include "Entity/Entity.h"

EndLevelScriptComponent::EndLevelScriptComponent()
{
}

void EndLevelScriptComponent::OnAttatchingToEntity()
{
	GetOwner()->RegisterMessageHandler(MessageType::COLLISION, Priority::HIGHEST,[&] (Message *msg) { return this->HandleCollisionMessage(msg); });
}

MessageHandling EndLevelScriptComponent::HandleCollisionMessage(Message *msg)
{	
	CollisionMessage* col = static_cast<CollisionMessage*>(msg);

  bool found_player = false;
  bool pass = false;
  for (auto entity : col->GetEntities())
  {
	  FactionComponent* faction = static_cast<FactionComponent*>(entity->GetComponent(ComponentType::FACTION));
	  if (faction == nullptr || faction->GetFaction() != Faction::PLAYER)
		  pass = true;
    else if (faction->GetFaction() == Faction::PLAYER)
		  found_player = true;
  }

  if (found_player)
	  GetOwner()->SendMessage(MessageFactory::CreateEndLevelMessage().release());
  return pass ? MessageHandling::PASS_FORWARD : MessageHandling::STOP_HANDLING;
}
