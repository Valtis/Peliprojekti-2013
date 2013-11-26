#pragma once
#include <memory>
#include <SDL.h>
class Entity;
class Message;
enum class CollisionSide;
enum class EntityType;
enum class Velocity;
typedef struct _CollisionHit CollisionHit;

namespace MessageFactory
{
  std::unique_ptr<Message> CreateLocationChangeMessage(double x, double y);
  std::unique_ptr<Message> CreateVelocityChangeMessage(double x, double y);
  std::unique_ptr<Message> CreateSetVelocityMessage(double x, double y);
  std::unique_ptr<Message> CreateSetVelocityMessage(double vel, Velocity dir);
  std::unique_ptr<Message> CreateCollisionMessage(CollisionHit *hit);
  std::unique_ptr<Message> CreateSpawnEntityMessage(EntityType type, Entity *spawner);
  std::unique_ptr<Message> CreateTerminateEntityMessage(Entity *entity);
  std::unique_ptr<Message> CreateNewTiledSpriteMessage(int baseID, int newID, int width, int height);
  std::unique_ptr<Message> CreateTakeDamageMessage();
  std::unique_ptr<Message> CreateEndLevelMessage();
  std::unique_ptr<Message> CreateEndGameMessage();


}
