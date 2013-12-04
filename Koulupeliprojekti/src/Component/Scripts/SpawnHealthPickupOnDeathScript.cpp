#include "Component/Scripts/SpawnHealthPickupOnDeathScript.h"
#include "Entity/Entity.h"
#include "Message/MessageFactory.h"
#include <ctime>
SpawnHealthPickupOnDeathScript::SpawnHealthPickupOnDeathScript(int dropChange) :
  m_dropChange(dropChange*100) 
{
  m_randomEngine.seed(time(nullptr));
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
  std::uniform_int_distribution<int> dist(0, 10000);
  if (dist(m_randomEngine) < m_dropChange)
  {
    auto spawnMsg = MessageFactory::CreateSpawnEntityMessage(EntityType::HEALTH_PICKUP, GetOwner());
    GetOwner()->SendMessage(spawnMsg.get());
  }
  


  GetOwner()->SendMessageUpwards(msg);
  return MessageHandling::STOP_HANDLING;
}