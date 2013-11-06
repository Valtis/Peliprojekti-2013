#include "Level/Level.h"
#include "Entity/Entity.h"
#include "Entity/EntityFactory.h"
#include "Collision/CollisionChecker.h"
#include "Physics/PhysicsHandler.h"
#include "Utility/LoggerManager.h"

#include "Message/TerminateEntityMessage.h"
#include "Message/SpawnEntityMessage.h"

#include "UI/Window/Window.h"

Level::Level()
{
  m_messageProcessor.RegisterMessageHandler(MessageType::SPAWN_ENTITY, Priority::HIGHEST, 
    [&](Message *msg) { return this->HandleEntitySpawning(msg); });
  m_messageProcessor.RegisterMessageHandler(MessageType::TERMINATE_ENTITY, Priority::HIGHEST, 
    [&](Message *msg) { return this->HandleEntityTermination(msg); });
  m_messageProcessor.RegisterMessageHandler(MessageType::END_LEVEL, Priority::HIGHEST,
    [&](Message *msg) { this->debugVictoryWindow->Activate(); return false; });

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
  HandlePendingDeletions();

  // physics check

  for (auto &entity : m_entities)
  {
    entity->Update(ticksPassed);
  }
  Physics::HandlePhysics(m_entities);
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
    LoggerManager::GetLog(LEVEL_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in Level::HandleEntitySpawning() - ignoring");
    return true;
  }

  AddEntity(EntityFactory::CreateEntity(static_cast<SpawnEntityMessage *>(msg)));
  return false;
}


bool Level::HandleEntityTermination(Message *msg)
{
  if (msg->GetType() != MessageType::TERMINATE_ENTITY)
  {
    LoggerManager::GetLog(LEVEL_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in Level::HandleEntityTermination() - ignoring");
    return true;
  }
  
  auto terminateMsg = static_cast<TerminateEntityMessage *>(msg);
  m_deletionList.push_back(terminateMsg->GetTerminateEntity());  
  return false;
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
