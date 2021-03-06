#include "Message/ScriptMessageInterface.h"
#include "Message/MessageFactory.h"
#include "Component/FactionComponent.h"
#include "Entity/Entity.h"


#include "Message/CollisionMessage.h"
#include "Message/StartBlinkingMessage.h"


namespace ScriptMessageInterface {
  void SendBlinkingMessage(Entity *entity, int duration) {
    auto message = MessageFactory::CreateStartBlinkingMessage(duration);
    entity->SendMessage(message.get());
  }

  void SendSpawnMessage(Entity *entity, int entityType) {
    auto message = MessageFactory::CreateSpawnEntityMessage(static_cast<EntityType>(entityType), entity);
    entity->SendMessage(message.get());
  }

  void SendAddHealthMessage(Entity *entity, int32_t amount) {
    auto message = MessageFactory::CreateAddHealthMessage(amount);
    entity->SendMessage(message.get());
  }

  void SendPlaySoundEffectMessage(Entity *entity, int32_t id) {
    auto message = MessageFactory::CreatePlaySoundMessage(static_cast<SoundEffectType>(id));
    entity->SendMessage(message.get());
  }

  void SendVisibilityMessage(Entity* entity, bool visibility) {
    auto message = MessageFactory::CreateSetGraphicsVisibilityMessage(visibility);
    entity->SendMessage(message.get());
  }

  void SendTakeDamageMessage(Entity *entity, int32_t amount) {
    auto message = MessageFactory::CreateTakeDamageMessage(amount);
    entity->SendMessage(message.get());
  }

  void SendTerminateEntityMessage(Entity *entity) {
    auto message = MessageFactory::CreateTerminateEntityMessage(entity);
    entity->SendMessage(message.get());
  }

  void SendImportantCharacterDeathMessage(Entity* entity) {
    auto message = MessageFactory::CreateImportantCharacterDeathMessage();
    entity->SendMessage(message.get());
  }

  void SendEndLevelMessage(Entity* entity) {
    auto message = MessageFactory::CreateEndLevelMessage();
    entity->SendMessage(message.get());
  }

  std::vector<Entity *> CollisionMessageGetFactions(Message *message) {
    auto colMsg = dynamic_cast<CollisionMessage *>(message);
    if (colMsg == nullptr) {
      throw std::runtime_error("Invalid pointer: Expected CollisionMessage but wasn't");
    }
    return colMsg->GetEntities();
  }

  int32_t BlinkMessageGetDuration(Message* message) {
    auto blinkMsg = dynamic_cast<StartBlinkingMessage *>(message);
    if (blinkMsg == nullptr) {
      throw std::runtime_error("Invalid pointer: Expected StartBlinkingMessage but wasn't");
    }
    return static_cast<int32_t>(blinkMsg->GetBlinkDuration());

  }

  int32_t GetHitType(Message* message) {
    auto colMsg = dynamic_cast<CollisionMessage *>(message);
    if (colMsg == nullptr) {
      throw std::runtime_error("Invalid pointer: Expected CollisionMessage but wasn't");
    }
    return static_cast<int32_t>(colMsg->GetHitType());
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