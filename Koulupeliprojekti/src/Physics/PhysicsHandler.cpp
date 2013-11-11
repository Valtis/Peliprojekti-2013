#include "Physics/PhysicsHandler.h"
#include "Component/PhysicsComponent.h"
#include "Component/VelocityComponent.h"
#include "Entity/Entity.h"
#include <SDL.h>

void HandleEntityPhysics(const std::unique_ptr<Entity> &entity)
{
  double x,y,vx,vy,max,max_x,max_y;
  PhysicsComponent *physics;
  VelocityComponent *velocity;

  physics =
    static_cast<PhysicsComponent *>(entity->GetComponent(ComponentType::PHYSICS));
  velocity =
    static_cast<VelocityComponent *>(entity->GetComponent(ComponentType::VELOCITY));
  if (physics == nullptr || velocity == nullptr)
    return;

  vx = velocity->GetVelocityX();
  vy = velocity->GetVelocityY();
  x = physics->GetAccelerationX();
  y = physics->GetAccelerationY();
  max = physics->GetTerminalVelocity();
  
  // Ensures maximum velocity can also be diagonal
  if (x == 0.0)
    max_x = 0.0;
  else if (y == 0.0)
    max_x = max;
  else
    max_x = max * (x / (x + y));
  
  if (y == 0.0)
    max_y = 0.0;
  else if (x == 0.0)
    max_y = max;
  else
    max_y = max * (y / (x + y));

  // Reverse if max negative for comparison
  if (max < 0.0)
  {
    max = -max;
    vx = -vx;
    vy = -vy;
  }

  if ((y != 0.0 && vy < max_y) || (x != 0.0 && vx < max_x))
    velocity->AddVelocity(x,y);
}

void Physics::HandlePhysics(const std::vector<std::unique_ptr<Entity>> &entities)
{
  std::vector<std::unique_ptr<Entity>>::const_iterator e;
  for (e = entities.begin(); e != entities.end(); e++)
    HandleEntityPhysics((*e));
}
