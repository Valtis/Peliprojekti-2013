#pragma once
#include <memory>
class Entity;
class SpawnEntityMessage;
class InputManager;

namespace EntityFactory
{
  std::unique_ptr<Entity> CreatePlayer(int x, int y, InputManager &input);
  std::unique_ptr<Entity> CreateFlyingEnemy(int x, int y, Entity *target);
  std::unique_ptr<Entity> CreateBlock(int x, int y, int tileid);
  std::unique_ptr<Entity> CreateCollisionBlock(int x, int y, int w, int h);
  std::unique_ptr<Entity> CreateEndLevelEntity(int x, int y);
  std::unique_ptr<Entity> CreateEntity(SpawnEntityMessage *msg);  
}
