#include "Level/Level.h"
#include "Entity/Entity.h"

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


void Level::Update(double ticks_passed)
{
  for (auto &entity : m_Entities)
  {
    entity->Update(ticks_passed);
  }
}