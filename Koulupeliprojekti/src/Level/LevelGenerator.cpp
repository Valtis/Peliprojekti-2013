#include "Level/LevelGenerator.h"



LevelGenerator::LevelGenerator() {}
LevelGenerator::~LevelGenerator() {}

std::vector<std::unique_ptr<Level>> LevelGenerator::generateLevels(InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera)
{
  std::vector<std::unique_ptr<Level>> levels;
  std::unique_ptr<Level> level(new Level);
  std::unique_ptr<InputComponent> ci(new InputComponent);
  ci->RegisterInputHandler(m_inputManager);
  std::unique_ptr<Entity> e = CreatePlayer(200002, 70, 60, 35, ci);
  camera->SetEntity(e.get());
  std::unique_ptr<AiComponent> ai(new FlyingAiComponent(e.get()));
  // Näiden kahden järjestyksellä väliä, ratkaise
  level->AddEntity(std::move(e));
  CreateEnemy(200028, 220, 600, 0, level, ai, ci);


  std::vector<MapRect> map = generateGrid(100, 100);
  for (MapRect r : map) {
    for (int i = r.x; i < (r.x + r.w); ++i) {
      for (int j = r.y; j < (r.y + r.h); ++j) {
        CreateBlock(200031, i*16, j*16, 16, level, false);
      }
    }
    CreateCollisionBlock(r.x*16, r.y*16, r.w*16, r.h*16, level);
  }

  CreateEndLevelEntity(200001, 200, 100, 50, level);
  levels.push_back(std::move(level));
  return levels;
}

std::vector<MapRect> LevelGenerator::generateGrid(int a, int b)
{
  std::vector<MapRect> p(4);
  MapRect r;
  MapRect r1;
  MapRect r2;
  MapRect r3;
  r.x = 0;
  r.y = 0;
  r.w = a;
  r.h = 1;

  r1.x = a;
  r1.y = 0;
  r1.w = 1;
  r1.h = b;

  r2.x = 0;
  r2.y = b;
  r2.w = a;
  r2.h = 1;

  r3.x = 0;
  r3.y = 0;
  r3.w = 1;
  r3.h = b;

  p.push_back(r);
  p.push_back(r1);
  p.push_back(r2);
  p.push_back(r3);

  return p;
}


std::unique_ptr<Entity> LevelGenerator::CreatePlayer(int frame, int x, int y, int size, std::unique_ptr<InputComponent>& ci)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  std::unique_ptr<VelocityComponent> v(new VelocityComponent);
  std::unique_ptr<CollisionComponent> c(new CollisionComponent());
  std::unique_ptr<FactionComponent> f(new FactionComponent(Faction::PLAYER));
  std::unique_ptr<PhysicsComponent> p(new PhysicsComponent);

  g->AddFrame(0, frame, 20);
  g->AddFrame(0, frame + 1, 20);
  g->AddFrame(0, frame + 2, 0);
  g->AddFrame(0, frame + 3, 20);


  e->AddComponent(ComponentType::GRAPHICS, std::move(g));
  l->SetLocation(x, y);
  e->AddComponent(ComponentType::LOCATION, std::move(l));
  e->AddComponent(ComponentType::INPUT, std::move(ci));
  e->AddComponent(ComponentType::VELOCITY, std::move(v));
  c->AddHitbox(0, 0, size, size, HitboxType::SOLID);
  e->AddComponent(ComponentType::COLLISION, std::move(c));
  e->AddComponent(ComponentType::FACTION, std::move(f));
  e->AddComponent(ComponentType::PHYSICS,std::move(p));
  return e;
}

void LevelGenerator::CreateEnemy(int frame, int x, int y, int size, std::unique_ptr<Level>& level,
                               std::unique_ptr<AiComponent>& ai, std::unique_ptr<InputComponent>& ci)
{
  std::unique_ptr<Entity> monster(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  std::unique_ptr<CollisionComponent> c(new CollisionComponent());
  std::unique_ptr<VelocityComponent> v(new VelocityComponent);
  std::unique_ptr<FactionComponent> f(new FactionComponent(Faction::ENEMY));

  g->AddFrame(0,frame);
  g->AddFrame(0, 200032);
  monster->AddComponent(ComponentType::GRAPHICS, std::move(g));

  l->SetLocation(x,y);
  monster->AddComponent(ComponentType::LOCATION, std::move(l));
  ci.reset(new InputComponent(-1));
  monster->AddComponent(ComponentType::INPUT, std::move(ci));

  monster->AddComponent(ComponentType::VELOCITY, std::move(v));
  monster->AddComponent(ComponentType::AI, std::move(ai));

  c->AddHitbox(0,0,70,35, HitboxType::SOLID);
  c->AddHitbox(0,0,70,35, HitboxType::TRIGGER);
  monster->AddComponent(ComponentType::COLLISION, std::move(c));

  monster->AddComponent(ComponentType::FACTION, std::move(f));
  level->AddEntity(std::move(monster));
}

void LevelGenerator::CreateBlock(int frame, int x, int y, int size, std::unique_ptr<Level>& level, bool hitbox)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);


  g->AddFrame(0, frame);
  e->AddComponent(ComponentType::GRAPHICS, std::move(g));

  l->SetLocation(x, y);
  e->AddComponent(ComponentType::LOCATION, std::move(l));

  if (hitbox) {
    std::unique_ptr<CollisionComponent> c(new CollisionComponent());
    c->AddHitbox(0, 0, size, size, HitboxType::SOLID);
    e->AddComponent(ComponentType::COLLISION, std::move(c));
  }
  level->AddStaticEntity(std::move(e));

}

void LevelGenerator::CreateCollisionBlock(int x, int y, int w, int h, std::unique_ptr<Level>& level)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  std::unique_ptr<CollisionComponent> c(new CollisionComponent());
  //std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  //g->AddFrame(0, 200013);
  //e->AddComponent(ComponentType::GRAPHICS, std::move(g));
  l->SetLocation(x, y);
  c->AddHitbox(0, 0, w, h, HitboxType::SOLID);
  e->AddComponent(ComponentType::COLLISION, std::move(c));
  e->AddComponent(ComponentType::LOCATION, std::move(l));
  level->AddStaticEntity(std::move(e));

}

void LevelGenerator::CreateEndLevelEntity(int frame, int x, int y, int size, std::unique_ptr<Level>& level)
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
  level->AddEntity(std::move(e));
}


