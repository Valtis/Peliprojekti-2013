#include "Component/Scripts/SpawnHealthPickupOnDeathScript.h"
#include "Entity/Entity.h"
#include "Message/MessageFactory.h"
#include <ctime>
#include <cstdlib>
SpawnHealthPickupOnDeathScript::SpawnHealthPickupOnDeathScript(int dropChange) :
  m_dropChange(dropChange*100) 
{
}

SpawnHealthPickupOnDeathScript::~SpawnHealthPickupOnDeathScript()
{

}

void SpawnHealthPickupOnDeathScript::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::TERMINATE_ENTITY, Priority::HIGHEST,
    [&](Message *msg) {return this->HandleTerminateMessage(msg); });
}

MessageHandling SpawnHealthPickupOnDeathScript::HandleTerminateMessage(Message *msg)
{
  if (rand() % 10000 <= m_dropChange)
  {
    auto spawnMsg = MessageFactory::CreateSpawnEntityMessage(EntityType::HEALTH_PICKUP, GetOwner());
    GetOwner()->SendMessage(spawnMsg.get());
  }
  
  GetOwner()->SendMessageUpwards(msg);
  return MessageHandling::STOP_HANDLING;
}