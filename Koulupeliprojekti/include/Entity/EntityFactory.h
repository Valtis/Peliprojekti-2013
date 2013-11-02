#pragma once
#include <memory>
class Entity;
class SpawnEntityMessage;
namespace EntityFactory
{
  std::unique_ptr<Entity> CreateEntity(SpawnEntityMessage *msg);  
}