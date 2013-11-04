#include "Level/LevelManager.h"
#include "Level/Level.h"
#include <stdexcept>
#include "Entity/Entity.h"
#include "Component/InputComponent.h"
#include "Component/GraphicsComponent.h"
#include "Component/LocationComponent.h"
#include "Component/VelocityComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/FactionComponent.h"
#include "Component/WalkingAiComponent.h"
#include <stdlib.h>
#include <time.h>


const int NO_ACTIVE_LEVEL = -1;
LevelManager::LevelManager() : m_currentLevel(NO_ACTIVE_LEVEL)
{

}

LevelManager::~LevelManager()
{

}

void LevelManager::Initialize(InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera)
{
  std::unique_ptr<Level> level(new Level);
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  g->AddFrame(0, 200002);


  e->AddComponent(ComponentType::GRAPHICS, std::move(g));
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  l->SetLocation(600, 500);
  e->AddComponent(ComponentType::LOCATION, std::move(l));
  std::unique_ptr<InputComponent> ci(new InputComponent);
  ci->RegisterInputHandler(m_inputManager);

  e->AddComponent(ComponentType::INPUT, std::move(ci));

  std::unique_ptr<VelocityComponent> v(new VelocityComponent);
  e->AddComponent(ComponentType::VELOCITY, std::move(v));

  std::unique_ptr<CollisionComponent> c(new CollisionComponent());
  c->AddHitbox(0, 0, 50, 50, HitboxType::OBJECT);
  e->AddComponent(ComponentType::COLLISION, std::move(c));
  std::unique_ptr<FactionComponent> f(new FactionComponent(Faction::PLAYER));
  e->AddComponent(ComponentType::FACTION, std::move(f));



  camera->SetEntity(e.get());

  level->AddEntity(std::move(e));

  // Monster creation:
  std::unique_ptr<Entity> monster(new Entity);
  g.reset(new GraphicsComponent);
  g->AddFrame(0,200007);
  monster->AddComponent(ComponentType::GRAPHICS, std::move(g));
  l.reset(new LocationComponent);
  l->SetLocation(700,700);
  monster->AddComponent(ComponentType::LOCATION, std::move(l));
  ci.reset(new InputComponent(-1));
  monster->AddComponent(ComponentType::INPUT, std::move(ci));
  v.reset(new VelocityComponent);
  monster->AddComponent(ComponentType::VELOCITY, std::move(v));
  std::unique_ptr<AiComponent> ai(new WalkingAiComponent);
  monster->AddComponent(ComponentType::AI, std::move(ai));
  c.reset(new CollisionComponent);
  c->AddHitbox(0,0,50,50, HitboxType::OBJECT);
  monster->AddComponent(ComponentType::COLLISION, std::move(c));
  level->AddEntity(std::move(monster));

  // Below is simple random level generation for testing purposes.
  int steps [3][2][2] = { { {1,0}, {0,0} }, { {1,-1}, {1,0} }, { {0,1},{1,1} }};
  srand (time(NULL));

  int startX = 500;
  int startY = 600;
  for (int i = 0; i < 40; ++i) {
    int ran = rand() % 3;
    /*
    int sX = steps[ran][0][0];
    int sY = steps[ran][0][1];
    int sX2 = steps[ran][1][0];
    int sY2 = steps[ran][1][1];
    */
    for (int j = 0; j < 2; ++j) {
      int sX = steps[ran][j][0];
      int sY = steps[ran][j][1];
      e.reset(new Entity);
      g.reset(new GraphicsComponent);
      g->AddFrame(0, 200000);
      e->AddComponent(ComponentType::GRAPHICS, std::move(g));
      l.reset(new LocationComponent);
      startX = startX + (sX * 50);
      startY = startY + (sY * 50);
      l->SetLocation(startX, startY);
      e->AddComponent(ComponentType::LOCATION, std::move(l));
      c.reset(new CollisionComponent());
      c->AddHitbox(0, 0, 50, 50, HitboxType::OBJECT);
      e->AddComponent(ComponentType::COLLISION, std::move(c));

      level->AddEntity(std::move(e));
    }
  }

  AddLevel(std::move(level));
  SetCurrentLevel(0);
}

void LevelManager::AddLevel(std::unique_ptr<Level> level)
{
  m_levels.push_back(std::move(level));
}

void LevelManager::SetCurrentLevel(int level)
{
  if (level < 0 || level >= static_cast<int>(m_levels.size()))
  {
    throw std::runtime_error("Level id out of bounds in LevelManager::SetCurrentLevel");
  }

  m_currentLevel = level;
}

void LevelManager::Update(double ticksPassed)
{
  if (m_currentLevel == NO_ACTIVE_LEVEL)
  {
    return;
  }

  m_levels[m_currentLevel]->Update(ticksPassed);
}

Level *LevelManager::GetCurrentLevel()
{
  return m_levels[m_currentLevel].get();
}
