#include "Component/Scripts/BulletScriptComponent.h"
#include "Entity/Entity.h"
#include "Utility/LoggerManager.h"
#include "Message/CollisionMessage.h"
#include "Component/FactionComponent.h"
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
    [&](Message *msg){ return this->HandleCollisionMessage(msg); });
}

MessageHandling BulletScriptComponent::HandleCollisionMessage(Message *msg)
{
  if (msg->GetType() != MessageType::COLLISION)
  {
    LoggerManager::GetLog(COMPONENT_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in BulletScriptComponent::HandleCollisionMessage() - ignoring");
    return MessageHandling::PASS_FORWARD;
  }

  auto colMsg = static_cast<CollisionMessage *> (msg);
  auto myFaction = static_cast<FactionComponent *>(GetOwner()->GetComponent(ComponentType::FACTION));

  std::vector<Entity *> targets;
  for (auto entity : colMsg->GetEntities())
  {

    auto otherFaction = static_cast<FactionComponent *>(entity->GetComponent(ComponentType::FACTION));

    if (myFaction == nullptr || otherFaction == nullptr ||
        myFaction->GetFaction() != otherFaction->GetFaction())
      targets.push_back(entity);
  }

  if (targets.size() > 0)
  {
    for (auto target : targets)
    {
      auto takeDamageMsg = MessageFactory::CreateTakeDamageMessage();
      GetOwner()->SendMessage(takeDamageMsg.get());
      target->SendMessage(takeDamageMsg.get());
    }
  }

  return MessageHandling::PASS_FORWARD;
}
