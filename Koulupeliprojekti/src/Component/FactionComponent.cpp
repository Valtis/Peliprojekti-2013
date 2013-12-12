#include "Component/FactionComponent.h"
#include "Message/QueryFactionMessage.h"
#include "Entity/Entity.h"
FactionComponent::FactionComponent() : m_faction(Faction::NONE)
{

}

FactionComponent::FactionComponent(Faction f) : m_faction(f)
{

}

void FactionComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::QUERY_FACTION, Priority::LOWEST,
    [=](Message *msg) { return this->HandleQueryFactionMessage(msg); });
}

FactionComponent::~FactionComponent()
{

}

MessageHandling FactionComponent::HandleQueryFactionMessage(Message *message)
{
  auto facMsg = static_cast<QueryFactionMessage *>(message);
  facMsg->SetFaction(m_faction);
  return MessageHandling::STOP_HANDLING;
}
