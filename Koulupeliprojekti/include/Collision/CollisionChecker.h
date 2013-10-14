#pragma once
#include <memory>
#include <vector>
class Entity;

namespace Collision
{
  void CheckCollisions(const std::vector<std::unique_ptr<Entity>> &entities);
}