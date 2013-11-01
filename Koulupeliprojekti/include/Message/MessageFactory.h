#pragma once
#include <memory>
#include <SDL.h>
class Entity;
class Message;
enum class EntityType;

namespace MessageFactory
{
  std::unique_ptr<Message> CreateLocationChangeMessage(double x, double y);
  std::unique_ptr<Message> CreateVelocityChangeMessage(double x, double y);
  std::unique_ptr<Message> CreateSetVelocityMessage(double x, double y);
  std::unique_ptr<Message> CreateCollisionMessage(Entity *entity, SDL_Rect &intersection);
  std::unique_ptr<Message> CreateSpawnEntityMessage(EntityType type, Entity *spawner);
  std::unique_ptr<Message> CreateTerminateEntityMessage(Entity *entity);
}