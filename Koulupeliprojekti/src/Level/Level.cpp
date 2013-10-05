#include "Level/Level.h"
#include "Entity/Entity.h"

Level::Level()
{

}

Level::~Level()
{

}

void Level::AddMessage(std::unique_ptr<Message> message)
{
  // do something
}


void Level::RegisterMessageHandler(MessageType type, MessageCallback callback)
{
  // do something
}
