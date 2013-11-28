#include "Level/LevelGenerator.h"
#include "Entity/EntityFactory.h"
#include "Graphics/Camera/EntityTrackingCamera.h"
#include "Entity/Entity.h"
#include "Level/Level.h"
#include "UI/Hud.h"
#include <ctime>

#define PLAYER_FRAME 200002
#define ENEMY_FRAME 200028
#define BLOCK_FRAME 400002 // 200031
#define END_FRAME 200001
#define TILESIZE 32

LevelGenerator::LevelGenerator() {}
LevelGenerator::~LevelGenerator() {}

std::vector<std::unique_ptr<Level>> LevelGenerator::GenerateLevels( InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera, Hud &hud )
{
  std::vector<std::unique_ptr<Level>> levels;
  std::unique_ptr<Level> level(new Level);

  std::unique_ptr<Entity> e = EntityFactory::CreatePlayer(PLAYER_FRAME, 60, 70, 35, m_inputManager);
  camera->SetEntity(e.get());
  hud.SetPlayer(e.get());

  level->AddEntity(std::move(EntityFactory::CreateFlyingEnemy(ENEMY_FRAME, 220, 600, 0, e.get())));
  level->AddEntity(std::move(e));
 
  std::vector<SDL_Rect> map = GenerateGround(50);
  for (SDL_Rect r : map) {
    for (int i = r.x; i < (r.x + r.w); ++i) {
      for (int j = r.y; j < (r.y + r.h); ++j) {
        level->AddStaticEntity(EntityFactory::CreateBlock(BLOCK_FRAME, i*TILESIZE, j*TILESIZE, TILESIZE));
      }
    }
    level->AddStaticEntity(EntityFactory::CreateCollisionBlock(r.x*TILESIZE, r.y*TILESIZE, r.w*TILESIZE, r.h*TILESIZE));
  }

  level->AddEntity(EntityFactory::CreateEndLevelEntity(END_FRAME, 200, 100, 50));
  levels.push_back(std::move(level));
  return levels;
}



std::vector<SDL_Rect> LevelGenerator::GenerateBorders(int a, int b)
{
  std::vector<SDL_Rect> p(4);
  SDL_Rect r;
  SDL_Rect r1;
  SDL_Rect r2;
  SDL_Rect r3;
  r.x = 0;
  r.y = 0;
  r.w = a;
  r.h = 1;

  r1.x = a;
  r1.y = 0;
  r1.w = 1;
  r1.h = b + 1;

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

std::vector<SDL_Rect> LevelGenerator::GenerateGround(int steps)
{
  srand(time(NULL));
  int changes[3][2] = {
    {1, -2},
    {2, 3},
    {3, 4}
  };
  std::vector<SDL_Rect> p(0);
  int startX = 1;
  int startY = 40;
  int widest = -10000;
  int lowest = -10000;

  SDL_Rect a;
  a.x = startX;
  a.y = startY;
  a.w = 1;
  for (int i = 0; i < steps; ++i) {
    
    int r = rand() % 100;
    if (r < 70) {
      // lets go straight
      a.w += 3;
      startX += 3;
    } else {
      // push old block for return
      p.push_back(a);
      // we go up or down
      auto& t = changes[rand()%3];
      startX += t[0];
      startY += t[1];
      // we start a new block here
      a.x = startX;
      a.y = startY;
      a.w = 1;
    }
    if (startX > widest) {
      widest = startX;
    }
    if (startY > lowest) {
      lowest = startY;
    }
  }
  // The final may or may not be pushed to return
  p.push_back(a);

  lowest = lowest + 3;
  for (auto& r : p) {
    r.h = lowest - r.y;
  }
  std::vector<SDL_Rect> grid = GenerateBorders(widest, lowest);
  p.insert( p.end(), grid.begin(), grid.end() );
  return p;
}
