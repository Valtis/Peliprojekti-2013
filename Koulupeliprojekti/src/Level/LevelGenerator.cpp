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
  std::vector<int> map = generateGrid(100, 100);
  for (int i = 0; i < 100; ++i) {
    for (int j = 0; j < 100; ++j) {
      if (map[100 * i + j] == 1) {
          CreateBlock(200031, i*16, j*16, 16, level, true);
      } else {
        CreateBlock(200029, i*16, j*16, 16, level, false);
      }
    }
  }
  CreateEndLevelEntity(200001, 200, 100, 50, level);

  levels.push_back(std::move(level));
  return levels;
}

std::vector<int> LevelGenerator::generateGrid(int a, int b)
{
  std::vector<int> p(a*b, 0);
  for (int i = 0; i < b; ++i) {
    p[b * 0 + i] = 1;
    p[b * (a-1) + i] = 1;
  }
  for (int i = 0; i < a; ++i) {
    p[b * i + 0] = 1;
    p[b * i + (b-1)] = 1;
  }
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
  c->AddHitbox(0, 0, size, size, HitboxType::OBJECT);
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
  monster->AddComponent(ComponentType::GRAPHICS, std::move(g));

  l->SetLocation(x,y);
  monster->AddComponent(ComponentType::LOCATION, std::move(l));
  ci.reset(new InputComponent(-1));
  monster->AddComponent(ComponentType::INPUT, std::move(ci));

  monster->AddComponent(ComponentType::VELOCITY, std::move(v));
  monster->AddComponent(ComponentType::AI, std::move(ai));

  c->AddHitbox(0,0,70,35, HitboxType::OBJECT);
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
    c->AddHitbox(0, 0, size, size, HitboxType::OBJECT);
    e->AddComponent(ComponentType::COLLISION, std::move(c));
  }
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
  c->AddHitbox(0,0,size,size,HitboxType::OBJECT);
  e->AddComponent(ComponentType::COLLISION, std::move(c));
  std::unique_ptr<Component> sc(new EndLevelScriptComponent);
  e->AddScript(std::move(sc));
  level->AddEntity(std::move(e));
}


