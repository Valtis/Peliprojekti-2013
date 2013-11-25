#include "Level/LevelGenerator.h"
#include "Entity/EntityFactory.h"
#include "Graphics/Camera/EntityTrackingCamera.h"
#include "Entity/Entity.h"
#include "Level/Level.h"
#include "UI/Hud.h"
#include <ctime>

#define PLAYER_FRAME 200002
#define ENEMY_FRAME 200028
#define BLOCK_FRAME 200031
#define END_FRAME 200001

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
        level->AddStaticEntity(EntityFactory::CreateBlock(BLOCK_FRAME, i*16, j*16, 16));
      }
    }
    level->AddStaticEntity(EntityFactory::CreateCollisionBlock(r.x*16, r.y*16, r.w*16, r.h*16));
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
  int straight[2] = {1 , 0};
  int changes[3][3][2] = {
    {
      {3, 2}, {1, 0}, {1, 0}
    },
    {
      {1, -2}, {1, 0}, {1, 0}
    },
    {
      {4, 3}, {1, 0}, {1, 0}
    }
  };
  std::vector<SDL_Rect> p(0);
  int startX = 1;
  int startY = 40;
  int widest = -10000;
  int lowest = -10000;


  for (int i = 0; i < steps; ++i) {

    SDL_Rect a;
    a.x = startX;
    a.y = startY;
    a.w = 1;
    int r = rand() % 100;
    if (r < 60) {
      // lets go straight
      for (int j = 0; j < 4; ++j) {
        startX += straight[0];
        startY += straight[1];
        a.w += 1;
      }
      p.push_back(a);
    } else {
      auto& t = changes[rand()%3];
      a.x = startX;
      a.y = startY;
      for (auto& t : changes[rand()%3]) {
        startX += t[0];
        startY += t[1];
        a.x = startX;
        a.y = startY;
        a.w = + 1;
        p.push_back(a);
      }
    }
    if (startX > widest) {
      widest = startX;
    }
    if (startY > lowest) {
      lowest = startY;
    }
  }
  lowest = lowest + 3;
  for (auto& r : p) {
    r.h = lowest - r.y;
  }
  std::vector<SDL_Rect> grid = GenerateBorders(widest, lowest);
  p.insert( p.end(), grid.begin(), grid.end() );
  return p;
}