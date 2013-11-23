#include "Entity/EntityFactory.h"
#include "Entity/Entity.h"
#include "Message/SpawnEntityMessage.h"
#include "Component/LocationComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/GraphicsComponent.h"
#include "Component/InputComponent.h"
#include "Component/VelocityComponent.h"
#include "Component/FactionComponent.h"
#include "Component/HealthComponent.h"
#include "Component/Scripts/BulletScriptComponent.h"
#include <string>
#include <SDL.h>
#include <stdexcept>
#include <sstream>

// todo - clean up
void CreateBullet(Entity *e, SpawnEntityMessage *msg)
{
  Entity *spawner = msg->Spawner();

  LocationComponent *spawnerLocation = static_cast<LocationComponent *>(spawner->GetComponent(ComponentType::LOCATION));
  FactionComponent *spawnerFaction = static_cast<FactionComponent *>(spawner->GetComponent(ComponentType::FACTION));
  SDL_assert(spawnerLocation != nullptr);

  Faction bulletFaction = Faction::NONE;
  if (spawnerFaction != nullptr)
  {
    bulletFaction = spawnerFaction->GetFaction();
  }

  double x = spawnerLocation->GetX();
  double y = spawnerLocation->GetY();
  Direction direction = spawnerLocation->GetDirection();

  std::unique_ptr<LocationComponent> location(new LocationComponent());
  location->SetLocation(x, y);
  location->SetDirection(spawnerLocation->GetDirection());

  std::unique_ptr<GraphicsComponent> graphics(new GraphicsComponent());
  graphics->AddFrame(0, 200007);

  std::unique_ptr<CollisionComponent> collision(new CollisionComponent);
  collision->AddHitbox(0, 0, 20, 20, HitboxType::TRIGGER);
  std::unique_ptr<VelocityComponent> velocity(new VelocityComponent);

  y = 0;
  x = 5;

  if (direction == Direction::LEFT)
  {
    x = -5;
  }
  velocity->SetVelocity(x, y);

  std::unique_ptr<FactionComponent> faction(new FactionComponent(bulletFaction));
  std::unique_ptr<HealthComponent> health(new HealthComponent(1, 1, 0));
  std::unique_ptr<BulletScriptComponent> script(new BulletScriptComponent);

  e->AddComponent(ComponentType::COLLISION, std::move(collision));
  e->AddComponent(ComponentType::GRAPHICS, std::move(graphics));
  e->AddComponent(ComponentType::LOCATION, std::move(location));
  e->AddComponent(ComponentType::VELOCITY, std::move(velocity));
  e->AddComponent(ComponentType::FACTION, std::move(faction));
  e->AddComponent(ComponentType::HEALTH, std::move(health));
  e->AddScript(std::move(script));
}

std::unique_ptr<Entity> EntityFactory::CreateEntity(SpawnEntityMessage *msg)
{
  std::stringstream ss;
  ss << (int)(msg->GetEntityType());
  std::unique_ptr<Entity> e(new Entity);

  switch (msg->GetEntityType())
  {
  case EntityType::BULLET:
    CreateBullet(e.get(), msg);
    break;
  default:
    throw std::runtime_error("Default case reached in EntityFactory::CreateEntity() - EntityType " +
      ss.str() + " received");
    break;
  }

  return e;
}
