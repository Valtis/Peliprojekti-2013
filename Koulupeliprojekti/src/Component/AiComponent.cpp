#include "Component/AiComponent.h"
#include "Entity/Entity.h"

AiComponent::AiComponent()
{
}
void AiComponent::OnAttatchingToEntity()
{
	GetOwner()->RegisterMessageHandler(MessageType::COLLISION, Priority::HIGHEST,
		[&](Message *msg) { return this->HandleCollisionMessage(msg); });
}
