#include "Level/LevelGenerator.h"
#include "Entity/EntityFactory.h"
#include "Graphics/Camera/EntityTrackingCamera.h"
#include "Entity/Entity.h"
#include "Level/Level.h"
#include "UI/Hud.h"
#include <ctime>

#define PLAYER_FRAME 200002 // 200002
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
 
  for (int i = 0; i < 10; ++i) {
    std::vector<SDL_Rect> room = GenerateRoom((15*i)+i,0,1); // hardcoded room width 15
    for (SDL_Rect r: room) {
      for (int j = r.x; j < (r.x + r.w); ++j) {
        for (int k = r.y; k < (r.y + r.h); ++k) {
	  level->AddStaticEntity(EntityFactory::CreateBlock(BLOCK_FRAME, j*TILESIZE, k*TILESIZE, TILESIZE));
	}
      }
      level->AddStaticEntity(EntityFactory::CreateCollisionBlock(r.x*TILESIZE, r.y*TILESIZE, r.w*TILESIZE, r.h*TILESIZE));
    }
  }
  level->AddEntity(EntityFactory::CreateEndLevelEntity(END_FRAME, 200, 100, 50));
  levels.push_back(std::move(level));
  return levels;
}

std::vector<SDL_Rect> LevelGenerator::GenerateRoom(int x, int y, int n)
{
  // rooms always 30 width and 20 height
  // x and y is the left corner
  // n is which type of room we generate

  int room_width = 15; int room_height = 10;
  std::vector<SDL_Rect> room(0);
  switch (n)
  {
    case 1:
      SDL_Rect p1; // Left wall
      p1.x = x; p1.y = y; p1.h = room_height - 4; p1.w = 1;
      SDL_Rect p2; // Ceiling
      p2.x = x + 1; p2.y = y; p2.h = 1; p2.w = room_width;
      SDL_Rect p3; // Right wall
      p3.x = x + room_width; p3.y = y +1; p3.h = room_height - 5; p3.w = 1;
      SDL_Rect p4; // Floor
      p4.x = x; p4.y = y + room_height; p4.h = 1; p4.w = room_width+1;
      room.push_back(p1); room.push_back(p2); room.push_back(p3); room.push_back(p4);
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    default:
      break;
  }
  return room;
}


