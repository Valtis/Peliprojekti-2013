#include "Message/MessageFactory.h"
#include "Message/LocationChangeMessage.h"
#include "Message/VelocityChangeMessage.h"
#include "Message/SetVelocityMessage.h"
#include "Message/CollisionMessage.h"
#include "Message/SpawnEntityMessage.h"
#include "Message/TerminateEntityMessage.h"
#include "Message/GenericMessageWithNoData.h"
#include "Message/NewTiledSpriteMessage.h"

std::unique_ptr<Message> MessageFactory::CreateLocationChangeMessage(double x, double y)
{
  return std::unique_ptr<Message>(new LocationChangeMessage(x, y));
}

std::unique_ptr<Message> MessageFactory::CreateVelocityChangeMessage(double x, double y)
{
  return std::unique_ptr<Message>(new VelocityChangeMessage(x, y));
}

std::unique_ptr<Message> MessageFactory::CreateSetVelocityMessage(double x, double y)
{
  return std::unique_ptr<Message>(new SetVelocityMessage(x, y, Velocity::XY));
}

std::unique_ptr<Message> MessageFactory::CreateSetVelocityMessage(double vel, Velocity dir)
{
  if (dir == Velocity::X)
  {
    return std::unique_ptr<Message>(new SetVelocityMessage(vel,0,dir));
  }
  else
  {
    return std::unique_ptr<Message>(new SetVelocityMessage(0, vel, dir));
  }
}

std::unique_ptr<Message> MessageFactory::CreateCollisionMessage(CollisionHit *hit)
{
  return std::unique_ptr<Message>(new CollisionMessage(hit));
}

std::unique_ptr<Message> MessageFactory::CreateSpawnEntityMessage(EntityType type, Entity *spawner)
{
  return std::unique_ptr<Message>(new SpawnEntityMessage(type, spawner));
}

std::unique_ptr<Message> MessageFactory::CreateTerminateEntityMessage(Entity *spawner)
{
  return std::unique_ptr<Message>(new TerminateEntityMessage(spawner));
}

std::unique_ptr<Message> MessageFactory::CreateNewTiledSpriteMessage(int baseID, int newID, int width, int height)
{
  return std::unique_ptr<Message>(new NewTiledSpriteMessage(baseID, newID, width, height));
}

std::unique_ptr<Message> MessageFactory::CreateTakeDamageMessage()
{
  return std::unique_ptr<Message>(new GenericMessageWithNoData(MessageType::TAKE_DAMAGE));
}

std::unique_ptr<Message> MessageFactory::CreateEndLevelMessage()
{
  return std::unique_ptr<Message>(new GenericMessageWithNoData(MessageType::END_LEVEL));
}

std::unique_ptr<Message> MessageFactory::CreateEndGameMessage()
{
  return std::unique_ptr<Message>(new GenericMessageWithNoData(MessageType::END_GAME));
}

std::unique_ptr<Message> CreateStartBlinkingMessage(double duration)
{
  return nullptr;
}

std::unique_ptr<Message> CreateSetGraphicsVisibilityMessage(bool visibility)
{
  return nullptr;
}