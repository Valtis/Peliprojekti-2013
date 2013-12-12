#include "Component/Scripts/DamageColliderScript.h"
#include "Entity/Entity.h"
#include "Message/CollisionMessage.h"
#include "Message/QueryFactionMessage.h"
#include "Message/MessageFactory.h"

void DamageColliderScript::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::COLLISION, Priority::HIGH,
    [=](Message *msg)
  {
    return this->HandleCollisionMessage(msg);
  });
}

MessageHandling DamageColliderScript::HandleCollisionMessage(Message *msg)
{
  auto colMsg = static_cast<CollisionMessage *>(msg);


  auto facQuery = MessageFactory::CreateQueryFactionMessage();

  bool myQueryWasAnswered = GetOwner()->SendMessage(facQuery.get());
  auto myFaction = facQuery->GetFaction();

  std::vector<Entity *> targets;
  auto takeDamageMsg = MessageFactory::CreateTakeDamageMessage();
  for (auto entity : colMsg->GetEntities())
  {
    bool otherQueryWasAnswered = entity->SendMessage(facQuery.get());
    auto otherFaction = facQuery->GetFaction();

    if (myQueryWasAnswered == false || otherQueryWasAnswered == false ||
      myFaction != otherFaction)
    {
      entity->SendMessage(takeDamageMsg.get());
    }
  }
  return MessageHandling::PASS_FORWARD;
}