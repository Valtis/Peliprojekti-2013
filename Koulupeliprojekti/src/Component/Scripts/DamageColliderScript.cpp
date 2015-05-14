#include "Component/Scripts/DamageColliderScript.h"
#include "Component/FactionComponent.h"
#include "Entity/Entity.h"
#include "Message/CollisionMessage.h"

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

  auto myFaction = static_cast<FactionComponent *>(GetOwner()->GetComponent(ComponentType::FACTION));

  std::vector<Entity *> targets;
  auto takeDamageMsg = MessageFactory::CreateTakeDamageMessage(1);
  for (auto entity : colMsg->GetEntities())
  {
    auto otherFaction = static_cast<FactionComponent *>(entity->GetComponent(ComponentType::FACTION));

    if (myFaction == nullptr || otherFaction == nullptr ||
      myFaction->GetFaction() != otherFaction->GetFaction())
    {
      
      entity->SendMessage(takeDamageMsg.get());
    }
  }
  return MessageHandling::PASS_FORWARD;
}