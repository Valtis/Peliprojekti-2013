#include "Level/Level.h"
#include "Entity/Entity.h"
#include "Entity/EntityFactory.h"
#include "Collision/CollisionChecker.h"
#include "Physics/PhysicsHandler.h"
#include "Utility/LoggerManager.h"

#include "Message/TerminateEntityMessage.h"
#include "Message/SpawnEntityMessage.h"

Level::Level()
{
  RegisterMessageHandler(MessageType::SPAWN_ENTITY, Priority::HIGHEST, 
    [&](Message *msg) { return this->HandleEntitySpawning(msg); });
  RegisterMessageHandler(MessageType::TERMINATE_ENTITY, Priority::HIGHEST, 
    [&](Message *msg) { return this->HandleEntityTermination(msg); });
}

Level::~Level()
{

}


void Level::Update(double ticksPassed)
{
  HandlePendingDeletions();

  // physics check
  Physics::HandlePhysics(m_entities);

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

const std::vector<std::unique_ptr<Entity>> &Level::GetStaticEntities()
{
  return m_staticEntities;
}

void Level::AddEntity(std::unique_ptr<Entity> e)
{
  e->SetParent(this);
  m_entities.push_back(std::move(e));
}

void Level::AddStaticEntity(std::unique_ptr<Entity> e)
{
  e->SetParent(this);
  m_staticEntities.push_back(std::move(e));
}

MessageHandling Level::HandleEntitySpawning(Message *msg)
{
  if (msg->GetType() != MessageType::SPAWN_ENTITY)
  {
    LoggerManager::GetLog(LEVEL_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in Level::HandleEntitySpawning() - ignoring");
    return MessageHandling::PASS_FORWARD;
  }

  AddEntity(EntityFactory::CreateEntity(static_cast<SpawnEntityMessage *>(msg)));
  return MessageHandling::STOP_HANDLING;
}


MessageHandling Level::HandleEntityTermination(Message *msg)
{
  if (msg->GetType() != MessageType::TERMINATE_ENTITY)
  {
    LoggerManager::GetLog(LEVEL_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in Level::HandleEntityTermination() - ignoring");
    return MessageHandling::PASS_FORWARD;
  }

  auto terminateMsg = static_cast<TerminateEntityMessage *>(msg);
  m_deletionList.push_back(terminateMsg->GetTerminateEntity());  
  return MessageHandling::STOP_HANDLING;
}

void Level::HandlePendingDeletions()
{
  for (auto entity : m_deletionList)
  {
    for (auto iter = m_entities.begin(); iter != m_entities.end(); ++iter)
    {
      if ((*iter).get() == entity)
      {
        m_entities.erase(iter);
        break;
      }
    }
  }
  m_deletionList.clear();
}
