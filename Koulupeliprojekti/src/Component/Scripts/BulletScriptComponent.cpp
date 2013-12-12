#include "Component/Scripts/BulletScriptComponent.h"
#include "Entity/Entity.h"
#include "Utility/LoggerManager.h"
#include "Message/CollisionMessage.h"
#include "Message/QueryFactionMessage.h"
#include "Message/MessageFactory.h"

BulletScriptComponent::BulletScriptComponent() : m_lifeTimeRemaining(100)
{

}

BulletScriptComponent::~BulletScriptComponent()
{

}
                            

void BulletScriptComponent::Update(double ticksPassed)
{
  m_lifeTimeRemaining -= ticksPassed;
  if (m_lifeTimeRemaining <= 0)
  {
    auto takeDamageMsg = MessageFactory::CreateTakeDamageMessage();
    GetOwner()->SendMessage(takeDamageMsg.get());
  }
}

void BulletScriptComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::COLLISION,  Priority::HIGHEST, 
    [=](Message *msg){ return this->HandleCollisionMessage(msg); });
}

MessageHandling BulletScriptComponent::HandleCollisionMessage(Message *msg)
{
  if (msg->GetType() != MessageType::COLLISION)
  {
    LoggerManager::GetLog(COMPONENT_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in BulletScriptComponent::HandleCollisionMessage() - ignoring");
    return MessageHandling::PASS_FORWARD;
  }

  auto colMsg = static_cast<CollisionMessage *> (msg);

  auto facQuery = MessageFactory::CreateQueryFactionMessage();
  if (!GetOwner()->SendMessage(facQuery.get()))
  {
    throw std::runtime_error("Bullet script attached to entity with no faction component");
  }
  auto myFaction = facQuery->GetFaction();

  std::vector<Entity *> targets;
  for (auto entity : colMsg->GetEntities())
  {
    if (!entity->SendMessage(facQuery.get()) || myFaction != facQuery->GetFaction())
    {
      auto takeDamageMsg = MessageFactory::CreateTakeDamageMessage();
      GetOwner()->SendMessage(takeDamageMsg.get());
    }

  }
  return MessageHandling::PASS_FORWARD;
}
