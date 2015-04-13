#include "Message/ScriptMessageInterface.h"
#include "Message/MessageFactory.h"
#include "Entity/Entity.h"

namespace ScriptMessageInterface {
void SendBlinkingMessage(Entity *entity, int duration) {
  //LoggerManager::GetLog("foobar.txt").AddLine(LogLevel::INFO, std::string("Invoking message creation with entity ")
    // + std::to_string(uint64_t(entity)) + " and duration  " + std::to_string(duration));
  auto message = MessageFactory::CreateStartBlinkingMessage(duration);
  entity->SendMessage(message.get());
}

}