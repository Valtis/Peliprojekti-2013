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