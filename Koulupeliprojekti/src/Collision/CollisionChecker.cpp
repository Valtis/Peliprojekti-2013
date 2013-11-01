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
  std::vector<SDL_Rect> cc_hitboxes, cc2_hitboxes;
  SDL_Rect intersectRect;
  std::vector<std::unique_ptr<Entity>>::const_iterator e;

  cc = static_cast<CollisionComponent *>(entity->GetComponent(ComponentType::COLLISION));
  if (!cc)
    return;

  cc_hitboxes = cc->GetHitboxes(HitboxType::OBJECT);
  for (e = _e; e != entities.end(); e++)
  {
    if (entity == (*e))
      continue;

    cc2 = static_cast<CollisionComponent *>((*e)->GetComponent(ComponentType::COLLISION));
    if (!cc2)
      continue;
    
    cc2_hitboxes = cc2->GetHitboxes(HitboxType::OBJECT);
    for (auto cc_hitbox : cc_hitboxes)
    {
      for (auto cc2_hitbox : cc2_hitboxes)
      {
        if (SDL_IntersectRect(&cc_hitbox,&cc2_hitbox,&intersectRect))
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
  }
}

void Collision::CheckCollisions(const std::vector<std::unique_ptr<Entity>> &entities)
{
  std::vector<std::unique_ptr<Entity>>::const_iterator e;
  for (e = entities.begin(); e != entities.end(); e++)
    CheckEntityCollision((*e), e, entities);
}

