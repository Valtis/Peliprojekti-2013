#include "Message/ScriptMessageInterface.h"
#include "Message/MessageFactory.h"
#include "Entity/Entity.h"
#include "VM/VM.h"
#include "Utility/LoggerManager.h"

void ScriptMessageInterface::SendBlinkingMessage(Entity *entity, int duration) {
  auto message = MessageFactory::CreateStartBlinkingMessage(duration);
  entity->SendMessage(message.get());
}