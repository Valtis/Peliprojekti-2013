#include "Component/Scripts/QuitGameOnDeathScript.h"
#include "Entity/Entity.h"
#include "Message/MessageFactory.h"
void QuitGameOnDeathScript::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::TERMINATE_ENTITY, Priority::HIGHEST, 
    [&](Message *msg) { return this->HandleTerminationMessage(msg); });
}

MessageHandling QuitGameOnDeathScript::HandleTerminationMessage(Message *msg)
{
  auto quitMsg = MessageFactory::CreateImportantCharacterDeathMessage();
  GetOwner()->SendMessage(quitMsg.get());
  return MessageHandling::STOP_HANDLING;
}