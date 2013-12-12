#include "Component/Scripts/EndLevelScriptComponent.h"
#include "Message/CollisionMessage.h"
#include "Message/MessageFactory.h"
#include "Message/QueryFactionMessage.h"
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

  auto facQuery = MessageFactory::CreateQueryFactionMessage();

  for (auto entity : col->GetEntities())
  {

    bool answered = GetOwner()->SendMessage(facQuery.get());
    auto faction = facQuery->GetFaction();
	  if (answered == false || faction != Faction::PLAYER)
		  pass = true;
    else if (faction == Faction::PLAYER)
		  found_player = true;
  }

  if (found_player)
	  GetOwner()->SendMessage(MessageFactory::CreateEndLevelMessage().release());
  return pass ? MessageHandling::PASS_FORWARD : MessageHandling::STOP_HANDLING;
}
