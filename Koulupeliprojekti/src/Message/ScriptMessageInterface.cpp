#include "Message/ScriptMessageInterface.h"
#include "Message/MessageFactory.h"
#include "CollisionMessage.h"
#include "Component/FactionComponent.h"
#include "Entity/Entity.h"

namespace ScriptMessageInterface {
  void SendBlinkingMessage(Entity *entity, int duration) {
    auto message = MessageFactory::CreateStartBlinkingMessage(duration);
    entity->SendMessage(message.get());
  }

  void SendSpawnMessage(Entity *entity, int entityType) {
    auto message = MessageFactory::CreateSpawnEntityMessage(static_cast<EntityType>(entityType), entity);
    entity->SendMessage(message.get());
  }

  void SendTakeDamageMessage(Entity *entity) {
    auto message = MessageFactory::CreateTakeDamageMessage();
    entity->SendMessage(message.get());
  }

  std::vector<Entity *> CollisionMessageGetFactions(Message *message) {
    auto colMsg = dynamic_cast<CollisionMessage *>(message);
    if (colMsg == nullptr) {
      throw std::runtime_error("Invalid pointer: Expected CollisionMessage but wasn't");
    }
    return colMsg->GetEntities();
  }

  int32_t GetFaction(Entity *entity) {
    if (entity == nullptr) {
      throw std::runtime_error("Nullpointer provided when requesting faction");
    }
    auto faction = static_cast<FactionComponent *>(entity->GetComponent(ComponentType::FACTION));
    if (faction == nullptr) {
      return -1;
    }

    return static_cast<int32_t>(faction->GetFaction());
  }

}