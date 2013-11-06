#include "Component/Scripts/EndLevelScriptComponent.h"
#include "Component/FactionComponent.h"
#include "Message/CollisionMessage.h"
#include "Entity/Entity.h"

EndLevelScriptComponent::EndLevelScriptComponent()
{
}

void EndLevelScriptComponent::OnAttatchingToEntity()
{
	GetOwner()->RegisterMessageHandler(MessageType::COLLISION, Priority::HIGH,[&] (Message *msg) { return this->HandleCollisionMessage(msg); });
}

bool EndLevelScriptComponent::HandleCollisionMessage(Message *msg)
{
	if (msg->GetType() != MessageType::COLLISION)
		return false;
	
	CollisionMessage* col = static_cast<CollisionMessage*>(msg);
	FactionComponent* faction = static_cast<FactionComponent*>(col->GetEntity()->GetComponent(ComponentType::FACTION));
	if (faction == nullptr)
		return true;

	if (faction->GetFaction() != Faction::PLAYER)
		return true;

	GetOwner()->SendMessage(MessageFactory::CreateEndLevelMessage().release());
	return false;
}
