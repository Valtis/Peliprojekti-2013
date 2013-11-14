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

  std::unique_ptr<Level> level(new Level);
  std::unique_ptr<Entity> e(new Entity);
  std::unique_ptr<GraphicsComponent> g(new GraphicsComponent);
  g->AddFrame(0, 200002);

  e->AddComponent(ComponentType::GRAPHICS, std::move(g));
  std::unique_ptr<LocationComponent> l(new LocationComponent);
  l->SetLocation(70, 600);
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

  std::unique_ptr<PhysicsComponent> p(new PhysicsComponent);
  e->AddComponent(ComponentType::PHYSICS,std::move(p));


  camera->SetEntity(e.get());


  std::unique_ptr<AiComponent> ai(new FlyingAiComponent(e.get()));
  // Näiden kahden järjestyksellä väliä, ratkaise
  level->AddEntity(std::move(e));


  CreateEnemy(200028, 220, 600, 0, level, ai, ci);

  // Below is simple random level generation for testing purposes.
  int steps [3][3][2] = { { {1,0}, {1,0}, {1,0} }, // straight
  { {0,-1}, {0,0}, {1,0} }, // up
  { {0,1}, {0,0}, {1,0} }}; // down
  // Different tile sprites:
  int tileSprites [5] = {200013, 200017,
    200018, 200024, 200026};
  srand (static_cast<unsigned int>(time(NULL)));

  int startX = 50;
  int startY = 700;
  for (int i = 0; i < NUMBEROFBLOCKS; ++i) {
    // pick up/down/straight on random
    int ran = rand() % 3;
    // foreach step
    for (int j = 0; j < 3; ++j) {
      int sX = steps[ran][j][0];
      int sY = steps[ran][j][1];
      startX = startX + (sX * TILESIZE);
      startY = startY + (sY * TILESIZE);
      CreateBlock(200013, startX, startY, TILESIZE, level);

    }
  }

  CreateEndLevelEntity(200001, startX, startY-50, 50, level);
  AddLevel(std::move(level));
  SetCurrentLevel(0);
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
