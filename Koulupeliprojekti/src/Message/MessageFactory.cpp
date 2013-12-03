#include "Message/MessageFactory.h"
#include "Message/SetLocationMessage.h"
#include "Message/LocationChangeMessage.h"
#include "Message/VelocityChangeMessage.h"
#include "Message/SetVelocityMessage.h"
#include "Message/CollisionMessage.h"
#include "Message/SpawnEntityMessage.h"
#include "Message/ResetEntityPositionMessage.h"
#include "Message/TerminateEntityMessage.h"
#include "Message/GenericMessageWithNoData.h"
#include "Message/NewTiledSpriteMessage.h"
#include "Message/StartBlinkingMessage.h"
#include "Message/SetGraphicsVisibilityMessage.h"
#include "Message/PlaySoundEffectMessage.h"

std::unique_ptr<Message> MessageFactory::CreateLocationChangeMessage(double x, double y)
{
  return std::unique_ptr<Message>(new LocationChangeMessage(x, y));
}

std::unique_ptr<Message> MessageFactory::CreateSetLocationMessage(double x, double y)
{
  return std::unique_ptr<Message>(new SetLocationMessage(x, y));
}

std::unique_ptr<Message> MessageFactory::CreateSetVelocityMessage(Direction x, Direction y)
{
  return std::unique_ptr<Message>(new SetVelocityMessage(x, y));
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

std::unique_ptr<Message> MessageFactory::CreateResetEntityPositionMessage(Entity *entity)
{
  return std::unique_ptr<Message>(new ResetEntityPositionMessage(entity));
}

std::unique_ptr<Message> MessageFactory::CreateNewTiledSpriteMessage(int baseID, int newID, 
                                                                     int width, int height)
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

std::unique_ptr<Message> MessageFactory::CreateStartBlinkingMessage(double duration)
{
  return std::unique_ptr<Message>(new StartBlinkingMessage(duration));
}

std::unique_ptr<Message> MessageFactory::CreateSetGraphicsVisibilityMessage(bool visibility)
{
  return std::unique_ptr<Message>(new SetGraphicsVisibilityMessage(visibility));
}

std::unique_ptr<Message> MessageFactory::CreatePlaySoundEffectMessage(int id)
{
  return std::unique_ptr<Message>(new PlaySoundEffectMessage(id));
}
