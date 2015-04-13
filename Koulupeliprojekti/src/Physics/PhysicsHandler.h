#pragma once
#include <memory>
#include <vector>
class Entity;

namespace Physics
{
  void HandlePhysics(const std::vector<std::unique_ptr<Entity>> &entities);
}