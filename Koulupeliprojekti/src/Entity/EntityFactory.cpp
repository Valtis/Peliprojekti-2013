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

std::unique_ptr<Entity> EntityFactory::CreatePlayer(int frame, int x, int y, int size, InputManager &input)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  std::unique_ptr<VelocityComponent> v(new VelocityComponent);
  std::unique_ptr<CollisionComponent> c(new CollisionComponent());
  std::unique_ptr<FactionComponent> f(new FactionComponent(Faction::PLAYER));
  std::unique_ptr<PhysicsComponent> p(new PhysicsComponent);
  std::unique_ptr<InputComponent> i(new InputComponent);
  std::unique_ptr<HealthComponent> h(new HealthComponent(4, 4, 3));
  i->RegisterInputHandler(input);

  g->AddFrame(0, frame, 20);
  g->AddFrame(0, frame + 1, 20);
  g->AddFrame(0, frame + 2, 0);
  g->AddFrame(0, frame + 3, 20);


  e->AddComponent(ComponentType::GRAPHICS, std::move(g));
  l->SetLocation(x, y);
  e->AddComponent(ComponentType::LOCATION, std::move(l));
  e->AddComponent(ComponentType::VELOCITY, std::move(v));
  e->AddComponent(ComponentType::INPUT, std::move(i));
  
  c->AddHitbox(0, 0, size, size, HitboxType::SOLID);
  e->AddComponent(ComponentType::COLLISION, std::move(c));
  e->AddComponent(ComponentType::FACTION, std::move(f));
  e->AddComponent(ComponentType::PHYSICS,std::move(p));
  e->AddComponent(ComponentType::HEALTH, std::move(h));
  return e;
}

std::unique_ptr<Entity> EntityFactory::CreateFlyingEnemy(int frame, int x, int y, int size, Entity *target)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  std::unique_ptr<CollisionComponent> c(new CollisionComponent());
  std::unique_ptr<VelocityComponent> v(new VelocityComponent);
  std::unique_ptr<FactionComponent> f(new FactionComponent(Faction::ENEMY));
  std::unique_ptr<FlyingAiComponent> ai(new FlyingAiComponent(target));
  std::unique_ptr<InputComponent> i(new InputComponent(-1));

  g->AddFrame(0,frame, 5);
  g->AddFrame(0, 200032, 5);
  e->AddComponent(ComponentType::GRAPHICS, std::move(g));

  l->SetLocation(x,y);
  e->AddComponent(ComponentType::LOCATION, std::move(l));
  i.reset(new InputComponent(-1));
  e->AddComponent(ComponentType::INPUT, std::move(i));

  e->AddComponent(ComponentType::VELOCITY, std::move(v));
  e->AddComponent(ComponentType::AI, std::move(ai));

  c->AddHitbox(0,0,70,35, HitboxType::SOLID);
  c->AddHitbox(0,0,70,35, HitboxType::TRIGGER);
  e->AddComponent(ComponentType::COLLISION, std::move(c));

  e->AddComponent(ComponentType::FACTION, std::move(f));
  return e;
}

std::unique_ptr<Entity> EntityFactory::CreateBlock(int frame, int x, int y, int size)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);


  g->AddFrame(0, frame);
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


std::unique_ptr<Entity> EntityFactory::CreateEndLevelEntity(int frame, int x, int y, int size)
{

  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  std::unique_ptr<CollisionComponent> c(new CollisionComponent());

  g->AddFrame(0,frame);
  e->AddComponent(ComponentType::GRAPHICS,std::move(g));
  l.reset(new LocationComponent);
  l->SetLocation(x,y);
  e->AddComponent(ComponentType::LOCATION, std::move(l));
  c.reset(new CollisionComponent);
  c->AddHitbox(0,0,size,size,HitboxType::TRIGGER);
  e->AddComponent(ComponentType::COLLISION, std::move(c));
  std::unique_ptr<Component> sc(new EndLevelScriptComponent);
  e->AddScript(std::move(sc));
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
  default:
    throw std::runtime_error("Default case reached in EntityFactory::CreateEntity() reached");
    break;
  }
  return e;

}
