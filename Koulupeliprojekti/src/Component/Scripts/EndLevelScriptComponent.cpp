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

MessageHandling EndLevelScriptComponent::HandleCollisionMessage(Message *msg)
{
	if (msg->GetType() != MessageType::COLLISION)
		return MessageHandling::STOP_HANDLING;
	
	CollisionMessage* col = static_cast<CollisionMessage*>(msg);
	FactionComponent* faction = static_cast<FactionComponent*>(col->GetEntity()->GetComponent(ComponentType::FACTION));
	if (faction == nullptr)
		return MessageHandling::PASS_FORWARD;

	if (faction->GetFaction() != Faction::PLAYER)
		return MessageHandling::PASS_FORWARD;

	GetOwner()->SendMessage(MessageFactory::CreateEndLevelMessage().release());
	return MessageHandling::STOP_HANDLING;
}
