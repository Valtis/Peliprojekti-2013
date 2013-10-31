#include "Component/AiComponent.h"
#include "Entity/Entity.h"

AiComponent::AiComponent()
{
}
void AiComponent::OnAttachingToEntity()
{
	GetOwner()->RegisterMessageHandler(MessageType::COLLISION, Priority::HIGH,
		[&](Message *msg) { return this->HandleCollisionMessage(msg); });
}
