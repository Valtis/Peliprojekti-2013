#include "Level/Level.h"
#include "Entity/Entity.h"
#include "Entity/EntityFactory.h"
#include "Collision/CollisionChecker.h"
#include "Utility/LoggerManager.h"

#include "Message/SpawnEntityMessage.h"


Level::Level()
{
  m_messageProcessor.RegisterMessageHandler(MessageType::SPAWN_ENTITY, Priority::HIGHEST, 
    [&](Message *msg) { return this->HandleEntitySpawning(msg); });
}

Level::~Level()
{

}

void Level::SendMessage(Message *message)
{
  m_messageProcessor.SendMessage(message);
}


void Level::RegisterMessageHandler(MessageType type, Priority priority, MessageCallback callback)
{
  m_messageProcessor.RegisterMessageHandler(type, priority, callback);
}


void Level::Update(double ticksPassed)
{
  for (auto &entity : m_entities)
  {
    entity->Update(ticksPassed);
  }
  Collision::CheckCollisions(m_entities);
}


const std::vector<std::unique_ptr<Entity>> &Level::GetEntities()
{
  return m_entities;
}

void Level::AddEntity(std::unique_ptr<Entity> e)
{
  e->SetParent(this);
  m_entities.push_back(std::move(e));
}

bool Level::HandleEntitySpawning(Message *msg)
{
  if (msg->GetType() != MessageType::SPAWN_ENTITY)
  {
    LoggerManager::GetLog(LEVEL_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in LocationComponent::HandleLocationChangeMessage() - ignoring");
    return true;
  }

  AddEntity(EntityFactory::CreateEntity(static_cast<SpawnEntityMessage *>(msg)));
  return false;
}
