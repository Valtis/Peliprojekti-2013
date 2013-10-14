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
    if (entity == e)
    {
      continue;
    }
    /*
      Get location component and collision component from both entities
      Check that the components aren't null
      Determine if the collision areas overlap
      Signal one or both entities that collision happened
      If signaled both, make sure that same collision isn't checked twice
    */
  }
}