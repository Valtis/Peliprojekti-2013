#pragma once
#include <memory>
#include <SDL.h>
#include "Sound/SoundEffectDefs.h"
class Entity;

class Message;
enum class CollisionSide;
enum class EntityType;
enum class Direction;
struct CollisionHit;
namespace MessageFactory
{
  std::unique_ptr<Message> CreateSetLocationMessage(double x, double y);
  std::unique_ptr<Message> CreateLocationChangeMessage(double x, double y);
  std::unique_ptr<Message> CreateSetVelocityMessage(Direction x, Direction y);
  std::unique_ptr<Message> CreateCollisionMessage(CollisionHit &hit);
  std::unique_ptr<Message> CreateSpawnEntityMessage(EntityType type, Entity *spawner);
  std::unique_ptr<Message> CreateTerminateEntityMessage(Entity *entity);
  std::unique_ptr<Message> CreateResetEntityPositionMessage(Entity *entity);
  std::unique_ptr<Message> CreateNewTiledSpriteMessage(int baseID, int newID, int width, int height);
  std::unique_ptr<Message> CreateTakeDamageMessage(int amount);
  std::unique_ptr<Message> CreateEndLevelMessage();
  std::unique_ptr<Message> CreateEndGameMessage();
  std::unique_ptr<Message> CreateStartBlinkingMessage(double duration);
  std::unique_ptr<Message> CreateSetGraphicsVisibilityMessage(bool visibility);
  std::unique_ptr<Message> CreateSoundManagerPlaySoundMessage(int id);
  std::unique_ptr<Message> CreatePlaySoundMessage(SoundEffectType id);
  std::unique_ptr<Message> CreateAddHealthMessage(int health);
  std::unique_ptr<Message> CreateImportantCharacterDeathMessage();
  std::unique_ptr<Message> CreateSetFireDirectionMessage(Direction direction);
  std::unique_ptr<Message> CreateChangeAnimationMessage(int animationID);
}
