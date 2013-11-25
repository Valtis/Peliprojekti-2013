#pragma once
#include <memory>
class Entity;
class SpawnEntityMessage;
class InputManager;

namespace EntityFactory
{
  std::unique_ptr<Entity> CreatePlayer(int frame, int x, int y, int size, InputManager &input);
  std::unique_ptr<Entity> CreateFlyingEnemy(int frame, int x, int y, int size, Entity *target);
  std::unique_ptr<Entity> CreateBlock(int frame, int x, int y, int size);
  std::unique_ptr<Entity> CreateCollisionBlock(int x, int y, int w, int h);
  std::unique_ptr<Entity> CreateEndLevelEntity(int frame, int x, int y, int size);
  std::unique_ptr<Entity> CreateEntity(SpawnEntityMessage *msg);  
}