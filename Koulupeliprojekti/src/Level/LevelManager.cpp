#include "Level/LevelManager.h"
#include "Level/Level.h"
#include <stdexcept>
#include "Entity/Entity.h"
#include "Message/MessageFactory.h"
#include "Component/InputComponent.h"
#include "Component/GraphicsComponent.h"
#include "Component/LocationComponent.h"
#include "Component/VelocityComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/FactionComponent.h"
#include "Component/FlyingAiComponent.h"
#include "Component/WalkingAiComponent.h"
#include "Component/FollowingAiComponent.h"
#include "Component/Scripts/EndLevelScriptComponent.h"
#include "Component/PhysicsComponent.h"
#include <stdlib.h>
#include <time.h>
#include "Level/LevelGenerator.h"
#include <vector>


const int NUMBEROFBLOCKS = 100;
const int TILESIZE = 30;
const int NO_ACTIVE_LEVEL = -1;
LevelManager::LevelManager() : m_currentLevel(NO_ACTIVE_LEVEL)
{
  RegisterMessageHandler(MessageType::END_LEVEL, Priority::HIGHEST,
    [&](Message *msg)
    {
      return this->HandleEndLevelMessage(msg);
    }
  );
}

LevelManager::~LevelManager()
{

}

void LevelManager::Initialize(InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera)
{
  LevelGenerator lg;
  std::vector<int> map = lg.generateLevel(100, 100);

  std::unique_ptr<Level> level(new Level);
  std::unique_ptr<InputComponent> ci(new InputComponent);
  ci->RegisterInputHandler(m_inputManager);
  std::unique_ptr<Entity> e = CreatePlayer(200002, 70, 60, 50, ci);

  camera->SetEntity(e.get());


  std::unique_ptr<AiComponent> ai(new FlyingAiComponent(e.get()));
  // Näiden kahden järjestyksellä väliä, ratkaise
  level->AddEntity(std::move(e));


  CreateEnemy(200028, 220, 600, 0, level, ai, ci);


  for (int i = 0; i < 100; ++i) {
    for (int j = 0; j < 100; ++j) {
      if (map[100 * i + j] == 1) {
        CreateBlock(200031, i*16, j*16, 16, level);
      }
    }
  }

  CreateEndLevelEntity(200001, 100, 100, 50, level);
  AddLevel(std::move(level));
  SetCurrentLevel(0);
}

std::unique_ptr<Entity> LevelManager::CreatePlayer(int frame, int x, int y, int size, std::unique_ptr<InputComponent>& ci)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  std::unique_ptr<VelocityComponent> v(new VelocityComponent);
  std::unique_ptr<CollisionComponent> c(new CollisionComponent());
  std::unique_ptr<FactionComponent> f(new FactionComponent(Faction::PLAYER));
  std::unique_ptr<PhysicsComponent> p(new PhysicsComponent);
  g->AddFrame(0, frame);
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

void LevelManager::CreateEnemy(int frame, int x, int y, int size, std::unique_ptr<Level>& level,
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

void LevelManager::CreateBlock(int frame, int x, int y, int size, std::unique_ptr<Level>& level)
{
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  std::unique_ptr<CollisionComponent> c(new CollisionComponent());

  g->AddFrame(0, frame);
  e->AddComponent(ComponentType::GRAPHICS, std::move(g));

  l->SetLocation(x, y);
  e->AddComponent(ComponentType::LOCATION, std::move(l));

  c->AddHitbox(0, 0, size, size, HitboxType::OBJECT);
  e->AddComponent(ComponentType::COLLISION, std::move(c));

  level->AddStaticEntity(std::move(e));

}

void LevelManager::CreateEndLevelEntity(int frame, int x, int y, int size, std::unique_ptr<Level>& level)
{
  /*
  e.reset(new Entity);
  g.reset(new GraphicsComponent);
  */
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

void LevelManager::AddLevel(std::unique_ptr<Level> level)
{
  level->SetParent(this);
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

MessageHandling LevelManager::HandleEndLevelMessage(Message *msg)
{
  // add player position reset here
  ++m_currentLevel;
  if (m_currentLevel >= m_levels.size())
  {
    --m_currentLevel;
    auto msg = MessageFactory::CreateEndGameMessage();
    SendMessage(msg.get());
  }
  return  MessageHandling::STOP_HANDLING;
}
