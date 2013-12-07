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
  if (!isLeft && !isRight)
  {
    // Intersect is inside, find closest opposing side
    isLeft = (box1.x + box1.w - isect.x > isect.x + isect.w - box1.x);
    isRight = !isLeft;
  }

  bool isUp = (box1.y == isect.y);
  bool isDown = (box1.y + box1.h == isect.y + isect.h);
  if (!isUp && !isDown)
  {
    // Intersect is inside, find closest opposing side
    isUp = (box1.y + box1.h - isect.y > isect.y + isect.h - box1.y);
    isDown = !isUp;
  }

  hit.h_side = CollisionSide::NONE;
  hit.v_side = CollisionSide::NONE;
  hit.point.x = 0;
  hit.point.y = 0;
  if (isect.w < isect.h && isLeft != isRight)
  {
    hit.h_side = isLeft ? CollisionSide::LEFT : CollisionSide::RIGHT;
    hit.point.x = isect.w;
  }
  else if (isUp != isDown)
  {
    hit.v_side = isUp ? CollisionSide::UP : CollisionSide::DOWN;
    hit.point.y = isect.h;
  }

  return hit;
}

void CheckEntityEntityCollision(const std::unique_ptr<Entity> &entity,
                                const std::unique_ptr<Entity> &target_entity,
                                CollisionHit &hit, bool &first_hit)
{
  if (entity == target_entity)
    return;

  CollisionComponent *cc =
    static_cast<CollisionComponent *>(entity->GetComponent(ComponentType::COLLISION));
  CollisionComponent *cc2 =
    static_cast<CollisionComponent *>(target_entity->GetComponent(ComponentType::COLLISION));
  
  if (cc == nullptr || cc2 == nullptr)
    return;

  std::vector<SDL_Rect> cc_hitboxes = cc->GetHitboxes(HitboxType::SOLID);
  if (cc_hitboxes.size() == 0)
    cc_hitboxes = cc->GetHitboxes(HitboxType::TRIGGER);
  std::vector<SDL_Rect> cc2_hitboxes = cc2->GetHitboxes(hit.hit_type);
  if (cc_hitboxes.size() == 0 || cc2_hitboxes.size() == 0)
    return;
  
  bool hit_detected = false;
  for (SDL_Rect box1 : cc_hitboxes)
  {
    for (SDL_Rect box2 : cc2_hitboxes)
    {
      SDL_Rect isect;
      if (SDL_IntersectRect(&box1,&box2,&isect))
      {
        CollisionHit new_hit = CheckHitboxes(box1,box2,isect);
        new_hit.hit_type = hit.hit_type;
        if (first_hit)
        {
          hit = new_hit;
          hit.isect = isect;
          first_hit = false;
        }
        else
        {
          if (new_hit.h_side != CollisionSide::NONE)
          {
            if (hit.point.x < new_hit.point.x &&
                (new_hit.h_side == hit.h_side ||
                 hit.h_side == CollisionSide::NONE))
            {
              hit.h_side = new_hit.h_side;
              hit.point.x = new_hit.point.x;
            }
            else if (new_hit.h_side != hit.h_side)
            {
              hit.h_side = CollisionSide::NONE;
              hit.point.x = 0;
            }
          }
            
          if (new_hit.v_side != CollisionSide::NONE)
          {
            if (hit.point.y < new_hit.point.y &&
                (new_hit.v_side == hit.v_side || 
                 hit.v_side == CollisionSide::NONE))
            {
              hit.v_side = new_hit.v_side;
              hit.point.y = new_hit.point.y;
            }
            else if (new_hit.v_side != hit.v_side)
            {
              hit.v_side = CollisionSide::NONE;
              hit.point.y = 0;
            }
          }

          if (isect.x < hit.isect.x)
            hit.isect.x = isect.x;
          if (isect.y < hit.isect.y)
            hit.isect.y = isect.y;

          if (isect.w > hit.isect.w)
            hit.isect.w = isect.w;
          if (isect.h > hit.isect.h)
            hit.isect.h = isect.h;
        }
        hit_detected = true;
      }
    }
  }
  if (hit_detected)
    hit.entities.push_back(target_entity.get());
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

    trigger_hit.h_side = CollisionSide::NONE;
    trigger_hit.v_side = CollisionSide::NONE;
    trigger_hit.point.x = 0;
    trigger_hit.point.y = 0;
    trigger_hit.hit_type = HitboxType::TRIGGER;
    
    bool first_hit_s = true, first_hit_t = true;
    for (auto e2 = entities.begin(); e2 != entities.end(); e2++)
    {
      CheckEntityEntityCollision((*e),(*e2),solid_hit,first_hit_s);
      CheckEntityEntityCollision((*e),(*e2),trigger_hit,first_hit_t);
    }
    for (auto s_e = staticEntities.begin(); s_e != staticEntities.end(); s_e++)
      CheckEntityEntityCollision((*e),(*s_e),solid_hit,first_hit_s);

    if (solid_hit.h_side != CollisionSide::NONE ||
        solid_hit.v_side != CollisionSide::NONE)
    {
      CollisionHit *hit_ptr = new CollisionHit(solid_hit);
      auto msg = MessageFactory::CreateCollisionMessage(hit_ptr);
      (*e)->SendMessage(msg.get());
    }

    if (trigger_hit.h_side != CollisionSide::NONE ||
        trigger_hit.v_side != CollisionSide::NONE)
    {
      CollisionHit *hit_ptr = new CollisionHit(trigger_hit);
      auto msg = MessageFactory::CreateCollisionMessage(hit_ptr);
      (*e)->SendMessage(msg.get());
    }
  }
}

