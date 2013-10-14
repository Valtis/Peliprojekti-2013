#include "Collision/CollisionChecker.h"
#include "Entity/Entity.h"

void Collision::CheckCollisions(const std::vector<std::unique_ptr<Entity>> &entities)
{
  for (auto &e : entities)
  {
    CheckEntityCollision(e, entities);
  }
}

void CheckEntityCollision(const std::unique_ptr<Entity> &entity, const std::vector<std::unique_ptr<Entity>> &entities)
{
  for (auto &e : entities)
  {
    
  }
}