#include "Message/MessageFactory.h"
#include "Message/LocationChangeMessage.h"
#include "Message/VelocityChangeMessage.h"
#include "Message/SetVelocityMessage.h"
#include "Message/CollisionMessage.h"
#include "Entity/Entity.h"

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
  return std::unique_ptr<Message>(new SetVelocityMessage(x, y));
}

std::unique_ptr<Message> MessageFactory::CreateCollisionMessage(Entity *entity, SDL_Rect &intersection)
{
  return std::unique_ptr<Message>(new CollisionMessage(entity,intersection));
}

