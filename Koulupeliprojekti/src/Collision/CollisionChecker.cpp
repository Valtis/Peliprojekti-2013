#include "Collision/CollisionChecker.h"
#include "Component/CollisionComponent.h"
#include "Entity/Entity.h"
#include <SDL.h>

void CheckEntityCollision(const std::unique_ptr<Entity> &entity,
                          const std::vector<std::unique_ptr<Entity>>::const_iterator _e,
                          const std::vector<std::unique_ptr<Entity>> &entities)
{
  CollisionComponent *cc, *cc2;
  SDL_Rect intersectRect;
  std::vector<std::unique_ptr<Entity>>::const_iterator e;

  cc = static_cast<CollisionComponent *>(entity->GetComponent(ComponentType::COLLISION));
  for (e = _e; e != entities.end(); e++)
  {
    if (entity == (*e))
      continue;

    cc2 = static_cast<CollisionComponent *>((*e)->GetComponent(ComponentType::COLLISION));
    if (SDL_IntersectRect(&(cc->GetHitbox()),&(cc2->GetHitbox()),&intersectRect))
    {
      cc->HandleCollision((*e).get(),intersectRect);
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

void Collision::CheckCollisions(const std::vector<std::unique_ptr<Entity>> &entities)
{
  std::vector<std::unique_ptr<Entity>>::const_iterator e;
  for (e = entities.begin(); e != entities.end(); e++)
    CheckEntityCollision((*e), e, entities);
}

