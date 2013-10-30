#include "Entity/EntityFactory.h"
#include "Entity/Entity.h"
#include "Message/SpawnEntityMessage.h"
#include "Component/LocationComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/GraphicsComponent.h"
#include "Component/InputComponent.h"
#include "Component/VelocityComponent.h"
#include <string>
#include <SDL.h>

void CreateBullet(Entity *e, SpawnEntityMessage *msg)
{
  Entity *spawner = msg->Spawner();

  LocationComponent *spawnerLocation = static_cast<LocationComponent *>(spawner->GetComponent(ComponentType::LOCATION));
  SDL_assert(spawnerLocation != nullptr);
  
  double x = spawnerLocation->GetX();
  double y = spawnerLocation->GetY();
  Direction direction = spawnerLocation->GetDirection();
  
  std::unique_ptr<LocationComponent> location(new LocationComponent());
  location->SetLocation(x, y);
  location->SetDirection(spawnerLocation->GetDirection());

  std::unique_ptr<GraphicsComponent> graphics(new GraphicsComponent());
  graphics->AddFrame(0, 200007);

  std::unique_ptr<CollisionComponent> collision(new CollisionComponent(10, 10));

  std::unique_ptr<VelocityComponent> velocity(new VelocityComponent);
  y = 0;
  x = 5;
  if (direction == Direction::LEFT)
  {
    x = -5;
  }
  velocity->SetVelocity(x, y);

  // faction component here

  e->AddComponent(ComponentType::COLLISION, std::move(collision));
  e->AddComponent(ComponentType::GRAPHICS, std::move(graphics));
  e->AddComponent(ComponentType::LOCATION, std::move(location));
  e->AddComponent(ComponentType::VELOCITY, std::move(velocity));
}

std::unique_ptr<Entity> EntityFactory::CreateEntity(SpawnEntityMessage *msg)
{
  std::unique_ptr<Entity> e(new Entity);
  
  switch (msg->GetEntityType())
  {
  case EntityType::BULLET:
    CreateBullet(e.get(), msg);
    break;
  default:
    throw std::runtime_error("Default case reached in EntityFactory::CreateEntity() - EntityType " + std::to_string((int)msg->GetEntityType()) + 
      "received");
    break;
  }

  return e;
}