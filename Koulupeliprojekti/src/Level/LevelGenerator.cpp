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
  // rooms always 15 width and 10 height
  // x and y is the left corner
  // n is which type of room we generate

  int room_width = 15; int room_height = 10;
  std::vector<SDL_Rect> room(0);
  SDL_Rect p1, p2, p3, p4, p5, p6;
  switch (n)
  {
    case 1: // room has opening to left and right
      // Left wall
      p1.x = x, p1.y = y, p1.h = room_height - 4, p1.w = 1;
      // Ceiling
      p2.x = x + 1, p2.y = y, p2.h = 1, p2.w = room_width;
      // Right wall
      p3.x = x + room_width, p3.y = y +1, p3.h = room_height - 5, p3.w = 1;
      // Floor
      p4.x = x, p4.y = y + room_height, p4.h = 1, p4.w = room_width+1;
      room.push_back(p1), room.push_back(p2), room.push_back(p3), room.push_back(p4);
      break;
    case 2: // room has opening left, right and down
      // Left wall
      p1.x = x, p1.y = y, p1.h = room_height - 4, p1.w = 1;
      // Ceiling
      p2.x = x + 1, p2.y = y, p2.h = 1, p2.w = room_width;
      // Right wall
      p3.x = x + room_width, p3.y = y +1, p3.h = room_height - 5, p3.w = 1;
      // Left floor
      p4.x = x, p4.y = y + room_height, p4.h = 1, p4.w = 6;
      // Right floor
      p5.x = x + 8, p5.y = y + room_height, p5.h = 1, p5.w = 8;
      room.push_back(p1), room.push_back(p2), room.push_back(p3);
      room.push_back(p4), room.push_back(p5);
      break;
    case 3: // opening left, right and up
      // Left ceiling
      p1.x = x, p1.y = y, p1.h = 1, p1.w = 6;
      // Right ceiling
      p2.x = x + 8, p2.y = y, p2.h = 1, p2.w = 8;
      // Right wall
      p3.x = x + room_width, p3.y = y +1, p3.h = room_height - 5, p3.w = 1;
      // Left wall
      p4.x = x, p4.y = y+1, p4.h = room_height - 5, p4.w = 1;
      // Floor
      p5.x = x, p5.y = y + room_height, p5.h = 1, p5.w = room_width+1;
      room.push_back(p1), room.push_back(p2), room.push_back(p3);
      room.push_back(p4), room.push_back(p5);
      break;
    case 4: // opening to right
      // Ceiling
      p1.x = x, p1.y = y, p1.h = 1, p1.w = room_width+1;
      // Left wall
      p2.x = x, p2.y = y+1, p2.h = room_height-1, p2.w = 1;
      // Floor
      p3.x = x, p3.y = y + room_height, p3.h = 1, p3.w = room_width+1;
      room.push_back(p1), room.push_back(p2), room.push_back(p3);
      break;
    case 5: // opening to left
      // Ceiling
      p1.x = x, p1.y = y, p1.h = 1, p1.w = room_width+1;
      // Right wall
      p2.x = x + room_width, p2.y = y+1, p2.h = room_height-1, p2.w = 1;
      // Floor
      p3.x = x, p3.y = y + room_height, p3.h = 1, p3.w = room_width+1;
      room.push_back(p1), room.push_back(p2), room.push_back(p3);
      break;
    default: // room 6 opening to every direction
      // Left ceiling
      p1.x = x, p1.y = y, p1.h = 1, p1.w = 6;
      // Right ceiling
      p2.x = x + 8, p2.y = y, p2.h = 1, p2.w = 8;
      // Right wall
      p3.x = x + room_width, p3.y = y +1, p3.h = room_height - 5, p3.w = 1;
      // Left wall
      p4.x = x, p4.y = y+1, p4.h = room_height - 5, p4.w = 1;
      // Left floor
      p5.x = x, p5.y = y + room_height, p5.h = 1, p5.w = 6;
      // Right floor
      p6.x = x + 8, p6.y = y + room_height, p6.h = 1, p6.w = 8;
      room.push_back(p1), room.push_back(p2), room.push_back(p3);
      room.push_back(p4), room.push_back(p5), room.push_back(p6);
      break;
  }
  return room;
}


