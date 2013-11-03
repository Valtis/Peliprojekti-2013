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
  l->SetLocation(600, 550);
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


  for (int i = 0; i < 30; ++i) {
    e.reset(new Entity);
    g.reset(new GraphicsComponent);
    g->AddFrame(0, 200000);
    e->AddComponent(ComponentType::GRAPHICS, std::move(g));
    l.reset(new LocationComponent);
    l->SetLocation(600 + (i * 50), 600);
    e->AddComponent(ComponentType::LOCATION, std::move(l));
    c.reset(new CollisionComponent());
    c->AddHitbox(0, 0, 50, 50, HitboxType::OBJECT);
    e->AddComponent(ComponentType::COLLISION, std::move(c));

    level->AddEntity(std::move(e));
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
