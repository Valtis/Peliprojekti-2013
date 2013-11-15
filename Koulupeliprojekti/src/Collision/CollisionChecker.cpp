#include <cmath>
#include "Collision/CollisionChecker.h"
#include "Component/CollisionComponent.h"
#include "Component/VelocityComponent.h"
#include "Message/CollisionMessage.h"
#include "Entity/Entity.h"
#include <SDL.h>

void CheckEntityEntityCollision(const std::unique_ptr<Entity> &entity,
                                const std::unique_ptr<Entity> &target_entity)
{
  double x,y;
  VelocityComponent *vc, *vc2;
  CollisionComponent *cc, *cc2;
  CollisionSide side1, side2, defaultSide;
  std::vector<SDL_Rect> cc_hitboxes, cc2_hitboxes;
  SDL_Rect intersectRect;
  
  cc = static_cast<CollisionComponent *>(entity->GetComponent(ComponentType::COLLISION));
  if (cc == nullptr)
    return;

  vc = nullptr;
  vc2 = nullptr;
  vc = static_cast<VelocityComponent *>(entity->GetComponent(ComponentType::VELOCITY));
  if (vc != nullptr)
  {
    // Use first entity's velocity
    x = vc->GetVelocityX();
    y = vc->GetVelocityY();
  }

  cc_hitboxes = cc->GetHitboxes(HitboxType::OBJECT);
  cc2 = static_cast<CollisionComponent *>(target_entity->GetComponent(ComponentType::COLLISION));
  if (cc2 == nullptr)
    return;
  if (vc == nullptr || (x == 0.0 && y == 0.0))
    vc2 = static_cast<VelocityComponent *>(target_entity->GetComponent(ComponentType::VELOCITY));
  
  if (vc2 != nullptr)
  {
    // User second entity's velocity
    x = vc2->GetVelocityX();
    y = vc2->GetVelocityY();
  }
  else if (vc == nullptr)
  {
    x = y = 0.0;
  }

  if (x == 0.0 && y == 0.0)
  {
    defaultSide = CollisionSide::NONE;
  }
  else if (y == 0.0)
  {
    if (x < 0.0)
      defaultSide = CollisionSide::LEFT;
    else
      defaultSide = CollisionSide::RIGHT;
  }
  else if (x == 0.0)
  {
    if (y < 0.0)
      defaultSide = CollisionSide::UP;
    else
      defaultSide = CollisionSide::DOWN;
  }
  else
  {
    if (SDL_fabs(y) > SDL_fabs(x))
    {
      // Moves faster vertically
      if (y < 0.0)
        defaultSide = CollisionSide::UP;
      else
        defaultSide = CollisionSide::DOWN;
    }
    else
    {
      // Moves faster horizontally
      if (x < 0.0)
        defaultSide = CollisionSide::LEFT;
      else
        defaultSide = CollisionSide::RIGHT;
    }
  }

  cc2_hitboxes = cc2->GetHitboxes(HitboxType::OBJECT);
  for (auto cc_hitbox : cc_hitboxes)
  {
    for (auto cc2_hitbox : cc2_hitboxes)
    {
      if (SDL_IntersectRect(&cc_hitbox,&cc2_hitbox,&intersectRect))
      {
        bool isLeft, isRight, isUp, isDown;
        isLeft = (intersectRect.x == cc_hitbox.x);
        isRight = ((intersectRect.x + intersectRect.w) == (cc_hitbox.x + cc_hitbox.w));
        isUp = (intersectRect.y == cc_hitbox.y);
        isDown = ((intersectRect.y + intersectRect.h) == (cc_hitbox.y + cc_hitbox.h));

        if ((!isRight && !isLeft && !isUp && !isDown) ||
            (isRight && isLeft) || (isUp && isDown) ||
            (isLeft && isUp) || (isLeft && isDown) ||
            (isRight && isUp) || (isRight && isDown))
          side1 = CollisionSide::NONE; // Is in the middle or surrounding or corner
        else if (isLeft)
          side1 = CollisionSide::LEFT;
        else if (isRight)
          side1 = CollisionSide::RIGHT;
        else if (isDown)
          side1 = CollisionSide::DOWN;
        else if (isUp)
          side1 = CollisionSide::UP;

        if (side1 == CollisionSide::NONE)
        {
          // Use default based on velocity
          side1 = defaultSide;
          if (side1 == CollisionSide::NONE)
          {
            // No default, use whichever side is nearest
            int leftDist, rightDist, upDist, downDist, min;
            leftDist = SDL_fabs(cc_hitbox.x - (cc2_hitbox.x + cc2_hitbox.w));
            rightDist = SDL_fabs((cc_hitbox.x + cc_hitbox.w) - cc2_hitbox.x);
            upDist = SDL_fabs(cc_hitbox.y - (cc2_hitbox.y + cc2_hitbox.h));
            downDist = SDL_fabs((cc_hitbox.y + cc_hitbox.h) - cc2_hitbox.y);
            min = std::min(leftDist,rightDist);
            min = std::min(min,upDist);
            min = std::min(min,downDist);

            if (min == rightDist)
              side1 = CollisionSide::RIGHT;
            else if (min == leftDist)
              side1 = CollisionSide::LEFT;
            else if (min == upDist)
              side1 = CollisionSide::UP;
            else if (min == downDist)
              side1 = CollisionSide::DOWN;
            else
              side1 = CollisionSide::RIGHT;
          }
        }


        switch(side1)
        {
        default: // If there is still somehow no side
          side1 = CollisionSide::RIGHT;
        case (CollisionSide::RIGHT):
          side2 = CollisionSide::LEFT;
          break;
        case (CollisionSide::LEFT):
          side2 = CollisionSide::RIGHT;
          break;
        case (CollisionSide::UP):
          side2 = CollisionSide::DOWN;
          break;
        case (CollisionSide::DOWN):
          side2 = CollisionSide::UP;
          break;
        }

        auto collisionMessage1 =
          MessageFactory::CreateCollisionMessage(entity.get(), intersectRect, side2);
        auto collisionMessage2 =
          MessageFactory::CreateCollisionMessage(target_entity.get(), intersectRect, side1);
        entity->SendMessage(collisionMessage2.get());
        target_entity->SendMessage(collisionMessage1.get());
      }
    }
  }
}

void CheckEntityCollisions(const std::unique_ptr<Entity> &entity,
                           const std::vector<std::unique_ptr<Entity>>::const_iterator _e,
                           const std::vector<std::unique_ptr<Entity>> &entities)
{
  for (auto e = _e; e != entities.end(); e++)
  {
    if (entity == (*e))
      continue;
    CheckEntityEntityCollision(entity,(*e));
  }
}

void Collision::CheckCollisions(const std::vector<std::unique_ptr<Entity>> &entities,
                                const std::vector<std::unique_ptr<Entity>> &staticEntities)
{
  for (auto e = entities.begin(); e != entities.end(); e++)
  {
    CheckEntityCollisions((*e), e, entities);
    for (auto s_e = staticEntities.begin(); s_e != staticEntities.end(); s_e++)
      CheckEntityEntityCollision((*e),(*s_e));
  }
}

