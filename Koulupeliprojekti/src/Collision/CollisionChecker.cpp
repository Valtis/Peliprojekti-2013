#include <cmath>
#include "Collision/CollisionChecker.h"
#include "Component/CollisionComponent.h"
#include "Component/VelocityComponent.h"
#include "Message/CollisionMessage.h"
#include "Entity/Entity.h"
#include <SDL.h>

CollisionHit CheckHitboxes(const SDL_Rect &box1, const SDL_Rect &box2, const SDL_Rect &isect)
{
  CollisionHit hit;

  bool isLeft = (box1.x == isect.x);
  bool isRight = (box1.x + box1.w == isect.x + isect.w);
  if (isLeft && isRight)
  {
    // Intersect covers from both sides, find shortest way out
    isLeft = (box1.x - box2.x > (box2.x + box2.w) - (box1.x + box1.w));
    isRight = !isLeft;
  }
  else if (!isLeft && !isRight)
  {
    // Intersect is inside, find closest opposing side
    isLeft = (box1.x + box1.w - isect.x > isect.x - box1.x);
    isRight = !isLeft;
  }

  bool isUp = (box1.y == isect.y);
  bool isDown = (box1.y + box1.h == isect.y + isect.h);
  if (isUp && isDown)
  {
    // Intersect covers from both sides, find shortest way out
    isUp = (box1.y - box2.y > (box2.y + box2.h) - (box1.y + box1.h));
    isDown = !isUp;
  }
  else if (!isUp && !isDown)
  {
    // Intersect is inside, find closest opposing side
    isUp = (box1.y + box1.h - isect.y > isect.y - box1.y);
    isDown = !isUp;
  }
  
  hit.h_side = isLeft ? CollisionSide::LEFT : CollisionSide::RIGHT;
  hit.v_side = isUp ? CollisionSide::UP : CollisionSide::DOWN;
  hit.point.x = isect.w;
  hit.point.y = isect.h;

  return hit;
}

void CheckEntityEntityCollision(const std::unique_ptr<Entity> &entity,
                                const std::unique_ptr<Entity> &target_entity,
                                HitboxType target_hitbox_type,
                                CollisionHit &hit)
{
  bool first_hit =
    (hit.h_side == CollisionSide::NONE &&
     hit.v_side == CollisionSide::NONE &&
     hit.point.x == 0 && hit.point.y == 0);

  if (entity == target_entity)
    return;

  CollisionComponent *cc =
    static_cast<CollisionComponent *>(entity->GetComponent(ComponentType::COLLISION));
  CollisionComponent *cc2 =
    static_cast<CollisionComponent *>(target_entity->GetComponent(ComponentType::COLLISION));
  VelocityComponent *vc =
    static_cast<VelocityComponent *>(entity->GetComponent(ComponentType::VELOCITY));
  if (cc == nullptr || cc2 == nullptr || vc == nullptr)
    return;

  std::vector<SDL_Rect> cc_hitboxes = cc->GetHitboxes(HitboxType::SOLID);
  if (cc_hitboxes.size() == 0)
    cc_hitboxes = cc->GetHitboxes(HitboxType::TRIGGER);
  std::vector<SDL_Rect> cc2_hitboxes = cc2->GetHitboxes(target_hitbox_type);
  if (cc_hitboxes.size() == 0 || cc2_hitboxes.size() == 0)
    return;
  
  double x = vc->GetVelocityX();
  double y = vc->GetVelocityY();

  for (SDL_Rect box1 : cc_hitboxes)
  {
    for (SDL_Rect box2 : cc2_hitboxes)
    {
      SDL_Rect isect;
      if (SDL_IntersectRect(&box1,&box2,&isect))
      {
        CollisionHit new_hit = CheckHitboxes(box1,box2,isect);
        if (first_hit)
        {
          hit = new_hit;
        }
        else
        {
          if (new_hit.h_side != hit.h_side)
          {
            hit.h_side = CollisionSide::NONE;
          }
          else if (hit.point.x < new_hit.point.x)
          {
            hit.point.x = new_hit.point.x;
          }

          if (new_hit.v_side != hit.v_side)
          {
            hit.v_side = CollisionSide::NONE;
          }
          else if (hit.point.y < new_hit.point.y)
          {
            hit.point.y = new_hit.point.y;
          }
        }
        hit.entities.push_back(target_entity.get());
      }
    }
  }
}

void Collision::CheckCollisions(const std::vector<std::unique_ptr<Entity>> &entities,
                                const std::vector<std::unique_ptr<Entity>> &staticEntities)
{
  for (auto e = entities.begin(); e != entities.end(); e++)
  {
    CollisionHit solid_hit, trigger_hit;
    solid_hit.h_side = CollisionSide::NONE;
    solid_hit.v_side = CollisionSide::NONE;
    solid_hit.point.x = 0;
    solid_hit.point.y = 0;
    solid_hit.hit_type = HitboxType::SOLID;

    trigger_hit = solid_hit;
    trigger_hit.hit_type = HitboxType::TRIGGER;

    for (auto e2 = entities.begin(); e2 != entities.end(); e2++)
    {
      CheckEntityEntityCollision((*e),(*e2),HitboxType::SOLID,solid_hit);
      CheckEntityEntityCollision((*e),(*e2),HitboxType::TRIGGER,trigger_hit);
    }
    for (auto s_e = staticEntities.begin(); s_e != staticEntities.end(); s_e++)
      CheckEntityEntityCollision((*e),(*s_e),HitboxType::SOLID,solid_hit);

    if (solid_hit.h_side != CollisionSide::NONE ||
        solid_hit.v_side != CollisionSide::NONE)
    {
      auto msg = MessageFactory::CreateCollisionMessage(solid_hit);
      (*e)->SendMessage(msg.get());
    }

    if (trigger_hit.h_side != CollisionSide::NONE ||
        trigger_hit.v_side != CollisionSide::NONE)
    {
      auto msg = MessageFactory::CreateCollisionMessage(trigger_hit);
      (*e)->SendMessage(msg.get());
    }
  }
}

