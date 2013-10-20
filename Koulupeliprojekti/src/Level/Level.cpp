#include "Level/Level.h"
#include "Entity/Entity.h"
#include "Collision/CollisionChecker.h"

Level::Level()
{

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
  Collision::CheckCollisions(m_Entities);
  for (auto &entity : m_Entities)
  {
    entity->Update(ticksPassed);
  }
}


const std::vector<std::unique_ptr<Entity>> &Level::GetEntities()
{
  return m_Entities;
}