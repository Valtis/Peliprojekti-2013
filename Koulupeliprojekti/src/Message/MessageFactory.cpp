#include "Message/MessageFactory.h"
#include "Message/LocationChangeMessage.h"
#include "Message/VelocityChangeMessage.h"
#include "Message/SetVelocityMessage.h"
#include "Message/CollisionMessage.h"
#include "Message/SpawnEntityMessage.h"
#include "Message/TerminateEntityMessage.h"
#include "Message/GenericMessageWithNoData.h"


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

std::unique_ptr<Message> MessageFactory::CreateCollisionMessage(Entity *entity,
                                                                SDL_Rect &intersection,
                                                                CollisionSide side)
{
  return std::unique_ptr<Message>(new CollisionMessage(entity,intersection,side));
}

std::unique_ptr<Message> MessageFactory::CreateSpawnEntityMessage(EntityType type, Entity *spawner)
{
  return std::unique_ptr<Message>(new SpawnEntityMessage(type, spawner));
}

std::unique_ptr<Message> MessageFactory::CreateTerminateEntityMessage(Entity *spawner)
{
  return std::unique_ptr<Message>(new TerminateEntityMessage(spawner));
}

std::unique_ptr<Message> MessageFactory::CreateTakeDamageMessage()
{
  return std::unique_ptr<Message>(new GenericMessageWithNoData(MessageType::TAKE_DAMAGE));
}

std::unique_ptr<Message> MessageFactory::CreateEndLevelMessage()
{
  return std::unique_ptr<Message>(new GenericMessageWithNoData(MessageType::END_LEVEL));
}
