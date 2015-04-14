#include "Message/ScriptMessageInterface.h"
#include "Message/MessageFactory.h"
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


}