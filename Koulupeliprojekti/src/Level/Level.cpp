#include "Level/Level.h"
#include "Entity/Entity.h"
#include "Entity/EntityFactory.h"
#include "Collision/CollisionChecker.h"
#include "Message/Commands/Command.h"
#include "Physics/PhysicsHandler.h"
#include "Utility/LoggerManager.h"

#include "Message/TerminateEntityMessage.h"
#include "Message/SpawnEntityMessage.h"
#include "Message/ResetEntityPositionMessage.h"

Level::Level()
{
  m_levelStartPoint.first = 0;
  m_levelStartPoint.second = 0;
  RegisterMessageHandler(MessageType::SPAWN_ENTITY, Priority::HIGHEST, 
    [&](Message *msg) { return this->HandleEntitySpawning(msg); });
  RegisterMessageHandler(MessageType::TERMINATE_ENTITY, Priority::HIGHEST, 
    [&](Message *msg) { return this->HandleEntityTermination(msg); });

  RegisterMessageHandler(MessageType::RESET_ENTITY_POSITION, Priority::HIGHEST, 
    [&](Message *msg) { return this->HandleEntityPositionReset(msg); });


}

Level::~Level()
{

}


void Level::Update(double ticksPassed)
{
  HandlePendingDeletions();

  for (auto &entity : m_entities)
  {
    entity->Update(ticksPassed);
  }

  Physics::HandlePhysics(m_entities);
  Collision::CheckCollisions(m_entities, m_staticCollidables);
}


const std::vector<std::unique_ptr<Entity>> &Level::GetEntities()
{
  return m_entities;
}

const std::vector<std::unique_ptr<Entity>> &Level::GetStaticEntities()
{
  return m_staticEntities;
}

const std::vector<std::unique_ptr<Entity>> &Level::GetStaticCollidables()
{
  return m_staticCollidables;
}

void Level::AddEntity(std::unique_ptr<Entity> e)
{
  e->SetParent(this);
  m_entities.push_back(std::move(e));
}

void Level::AddStaticEntity(std::unique_ptr<Entity> e)
{
  e->SetParent(this);
  Component* collision = e->GetComponent(ComponentType::COLLISION);
  if(collision != nullptr)
  {
    m_staticCollidables.push_back(std::move(e));
  }
  else
  {
    m_staticEntities.push_back(std::move(e));
  }
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
    LoggerManager::GetLog(LEVEL_LOG).AddLine(LogLevel::WARNING,
        "Invalid message type received in Level::HandleEntityTermination() - ignoring");
    return MessageHandling::PASS_FORWARD;
  }

  auto terminateMsg = static_cast<TerminateEntityMessage *>(msg);
  m_deletionList.push_back(terminateMsg->GetTerminateEntity());  
  return MessageHandling::STOP_HANDLING;
}

MessageHandling Level::HandleEntityPositionReset(Message *msg)
{
  auto resetMsg = static_cast<ResetEntityPositionMessage *>(msg);

  auto locationMsg = MessageFactory::CreateSetLocationMessage(m_levelStartPoint.first, m_levelStartPoint.second);
  resetMsg->GetEntity()->SendMessage(locationMsg.get());
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
