#include "Entity/EntityFactory.h"
#include "Entity/Entity.h"
#include "UI/InputManager.h"
#include "Message/SpawnEntityMessage.h"
#include "Component/FlyingAiComponent.h"
#include "Component/LocationComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/GraphicsComponent.h"
#include "Component/InputComponent.h"
#include "Component/VelocityComponent.h"
#include "Component/FactionComponent.h"
#include "Component/HealthComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/Scripts/BulletScriptComponent.h"
#include "Component/Scripts/EndLevelScriptComponent.h"
#include "Component/Scripts/DamageColliderScript.h"
#include "Component/Scripts/BlinkScript.h"
#include "Component/Scripts/TempInvulnerabilityScript.h"
#include "Component/Scripts/HealthPickupScript.h"
#include "Component/Scripts/SpawnHealthPickupOnDeathScript.h"
#include "Component/Scripts/QuitGameOnDeathScript.h"
#include <string>
#include <SDL.h>
#include <stdexcept>
#include <sstream>


// sobadsobadsobad
void CreateBullet(Entity *e, SpawnEntityMessage *msg)
{
  const int bulletHitboxSize = 20;
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
  double speed = 10.0;
  double xVel = 0;
  double yVel = 0;
  
  Direction direction = spawnerLocation->GetFiringDirection();

  switch (direction)
  {
    case Direction::RIGHT:
       x += bulletHitboxSize;
       xVel = speed;
      break;
    case Direction::LEFT:
      x -= bulletHitboxSize;
      xVel = -speed;
      break;
    case Direction::UP:
      y -= bulletHitboxSize;
      yVel = -speed;
      break;
    case Direction::DOWN:
      y += bulletHitboxSize;
      yVel = speed;
      break;
  default:
    break;
  }
  
  std::unique_ptr<LocationComponent> location(new LocationComponent());
  location->SetLocation(x, y);
  location->SetDirection(direction);

  std::unique_ptr<GraphicsComponent> graphics(new GraphicsComponent());
  graphics->AddFrame(0, 200007);

  std::unique_ptr<CollisionComponent> collision(new CollisionComponent);
  collision->AddHitbox(0, 0, bulletHitboxSize, bulletHitboxSize, HitboxType::TRIGGER);
  std::unique_ptr<VelocityComponent> velocity(new VelocityComponent(0, 0));

  
  



  velocity->SetVelocity(xVel, yVel);

  std::unique_ptr<FactionComponent> faction(new FactionComponent(bulletFaction));
  std::unique_ptr<HealthComponent> health(new HealthComponent(1, 1, 0));
  
 

  e->AddComponent(ComponentType::COLLISION, std::move(collision));
  e->AddComponent(ComponentType::GRAPHICS, std::move(graphics));
  e->AddComponent(ComponentType::LOCATION, std::move(location));
  e->AddComponent(ComponentType::VELOCITY, std::move(velocity));
  e->AddComponent(ComponentType::FACTION, std::move(faction));
  e->AddComponent(ComponentType::HEALTH, std::move(health));
  e->AddScript(std::unique_ptr<BulletScriptComponent>(new BulletScriptComponent));
  e->AddScript(std::unique_ptr<DamageColliderScript>(new DamageColliderScript));
}

void CreateHealthPickup(Entity *e, SpawnEntityMessage *msg)
{
  const int hitboxSize = 20;
  Entity *spawner = msg->Spawner();

  LocationComponent *spawnerLocation = static_cast<LocationComponent *>(spawner->GetComponent(ComponentType::LOCATION));
 
  double x = spawnerLocation->GetX();
  double y = spawnerLocation->GetY();

  e->AddComponent(ComponentType::LOCATION, std::unique_ptr<Component>(new LocationComponent(x, y)));
  e->AddComponent(ComponentType::VELOCITY, std::unique_ptr<Component>(new VelocityComponent(0, 0)));
  e->AddComponent(ComponentType::PHYSICS, std::unique_ptr<Component>(new PhysicsComponent));
  
  std::unique_ptr<GraphicsComponent> graphics(new GraphicsComponent());
  
  graphics->AddFrame(0, 200000);

  std::unique_ptr<CollisionComponent> collision(new CollisionComponent);
  collision->AddHitbox(0, 0, hitboxSize, hitboxSize, HitboxType::TRIGGER);
  

  e->AddComponent(ComponentType::COLLISION, std::move(collision));
  e->AddComponent(ComponentType::GRAPHICS, std::move(graphics));
  e->AddScript(std::unique_ptr<Component>(new HealthPickupScript(1)));
}

std::unique_ptr<Entity> EntityFactory::CreatePlayer(int x, int y, InputManager &input)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  std::unique_ptr<VelocityComponent> v(new VelocityComponent(5, 13));
  std::unique_ptr<CollisionComponent> c(new CollisionComponent());
  std::unique_ptr<FactionComponent> f(new FactionComponent(Faction::PLAYER));
  std::unique_ptr<PhysicsComponent> p(new PhysicsComponent);
  std::unique_ptr<InputComponent> i(new InputComponent);
  std::unique_ptr<HealthComponent> h(new HealthComponent(4, 4, 3));
  i->RegisterInputHandler(input);

  g->AddFrame(0, 200002, 20);
  g->AddFrame(0, 200002 + 1, 20);
  g->AddFrame(0, 200002 + 2, 20);
  g->AddFrame(0, 200002 + 3, 20);


  e->AddComponent(ComponentType::GRAPHICS, std::move(g));
  l->SetLocation(x, y);
  e->AddComponent(ComponentType::LOCATION, std::move(l));
  e->AddComponent(ComponentType::VELOCITY, std::move(v));
  e->AddComponent(ComponentType::INPUT, std::move(i));
  
  c->AddHitbox(0, 0, 35, 35, HitboxType::SOLID);
  e->AddComponent(ComponentType::COLLISION, std::move(c));
  e->AddComponent(ComponentType::FACTION, std::move(f));
  e->AddComponent(ComponentType::PHYSICS,std::move(p));
  e->AddComponent(ComponentType::HEALTH, std::move(h));
  e->AddScript(std::unique_ptr<Component>(new TempInvulnerabilityScript(60)));
  e->AddScript(std::unique_ptr<Component>(new BlinkScript(5)));
  e->AddScript(std::unique_ptr<Component>(new QuitGameOnDeathScript));

  return e;
}

std::unique_ptr<Entity> BaseEnemy(int x, int y)
{
  std::unique_ptr<Entity> e(new Entity);
  e->AddComponent(ComponentType::COLLISION,std::unique_ptr<CollisionComponent>(new CollisionComponent));
  e->AddComponent(ComponentType::LOCATION,  std::unique_ptr<LocationComponent>(new LocationComponent(x, y)));
  e->AddComponent(ComponentType::GRAPHICS, std::unique_ptr<GraphicsComponent>(new GraphicsComponent));
  e->AddComponent(ComponentType::VELOCITY, std::unique_ptr<VelocityComponent>(new VelocityComponent(3, 3)));
  e->AddComponent(ComponentType::HEALTH, std::unique_ptr<Component>(new HealthComponent(3, 3, 0)));
  e->AddComponent(ComponentType::INPUT,  std::unique_ptr<InputComponent>(new InputComponent(-1)));
  e->AddComponent(ComponentType::FACTION, std::unique_ptr<FactionComponent>(new FactionComponent(Faction::ENEMY)));
  e->AddScript(std::unique_ptr<DamageColliderScript>(new DamageColliderScript));
  e->AddScript(std::unique_ptr<TempInvulnerabilityScript>(new TempInvulnerabilityScript(2)));
  return e;
}

std::unique_ptr<Entity> EntityFactory::CreateFlyingEnemy(int x, int y, Entity *target)
{
  
  auto e = BaseEnemy(x, y);
  e->AddComponent(ComponentType::AI, std::unique_ptr<FlyingAiComponent>(new FlyingAiComponent(target)));

  auto g = static_cast<GraphicsComponent *>(e->GetComponent(ComponentType::GRAPHICS));
  auto c = static_cast<CollisionComponent *>(e->GetComponent(ComponentType::COLLISION));
  g->AddFrame(0, 200028, 5);
  g->AddFrame(0, 200032, 5);

  c->AddHitbox(0,0,70,35, HitboxType::SOLID);
  c->AddHitbox(0,0,70,35, HitboxType::TRIGGER);
  
  e->AddScript(std::unique_ptr<Component>(new SpawnHealthPickupOnDeathScript(25)));
  return e;
}

std::unique_ptr<Entity> EntityFactory::CreateBlock(int x, int y, int tileid)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);


  g->AddFrame(0, tileid); // 400005
  e->AddComponent(ComponentType::GRAPHICS, std::move(g));

  l->SetLocation(x, y);
  e->AddComponent(ComponentType::LOCATION, std::move(l));

  return e;
}

std::unique_ptr<Entity> EntityFactory::CreateCollisionBlock(int x, int y, int w, int h)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  std::unique_ptr<CollisionComponent> c(new CollisionComponent());


  l->SetLocation(x, y);
  c->AddHitbox(0, 0, w, h, HitboxType::SOLID);
  e->AddComponent(ComponentType::COLLISION, std::move(c));
  e->AddComponent(ComponentType::LOCATION, std::move(l));
  return e;

}

#include "Message/MessageFactory.h"


std::unique_ptr<Entity> EntityFactory::CreateEndLevelEntity(int x, int y)
{

  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  g->AddFrame(0, 200001);
  e->AddComponent(ComponentType::GRAPHICS,std::move(g));

  std::unique_ptr<CollisionComponent> c(new CollisionComponent());
  c->AddHitbox(0, 0, 50, 50,HitboxType::TRIGGER);
  e->AddComponent(ComponentType::COLLISION, std::move(c));

  e->AddComponent(ComponentType::LOCATION, std::unique_ptr<LocationComponent>(new LocationComponent(x, y)));

  e->AddScript(std::unique_ptr<Component>(new EndLevelScriptComponent));
  return e;
}


std::unique_ptr<Entity> EntityFactory::CreateEntity(SpawnEntityMessage *msg)
{
  std::unique_ptr<Entity> e(new Entity);

  switch (msg->GetEntityType())
  {
  case EntityType::BULLET:
    CreateBullet(e.get(), msg);
    break;
  case EntityType::HEALTH_PICKUP:
    CreateHealthPickup(e.get(), msg);
    break;
  default:
    throw std::runtime_error("Default case reached in EntityFactory::CreateEntity() reached");
    break;
  }
  return e;

}
