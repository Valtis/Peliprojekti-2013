#include "Collision/CollisionChecker.h"
#include "Component/CollisionComponent.h"
#include "Message/CollisionMessage.h"
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
  if (!cc)
    return;

  for (e = _e; e != entities.end(); e++)
  {
    if (entity == (*e))
      continue;

    cc2 = static_cast<CollisionComponent *>((*e)->GetComponent(ComponentType::COLLISION));
    if (cc2 && SDL_IntersectRect(&(cc->GetHitbox()),&(cc2->GetHitbox()),&intersectRect))
    {
      auto collisionMessage1 =
        MessageFactory::CreateCollisionMessage(entity.get(), intersectRect);
      auto collisionMessage2 =
        MessageFactory::CreateCollisionMessage((*e).get(), intersectRect);
      entity->SendMessage(collisionMessage1.get());
      (*e)->SendMessage(collisionMessage2.get());
    }
  }
}

void Collision::CheckCollisions(const std::vector<std::unique_ptr<Entity>> &entities)
{
  std::vector<std::unique_ptr<Entity>>::const_iterator e;
  for (e = entities.begin(); e != entities.end(); e++)
    CheckEntityCollision((*e), e, entities);
}

