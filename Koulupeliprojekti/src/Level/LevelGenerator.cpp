#include "Level/LevelGenerator.h"
#include "Entity/EntityFactory.h"
#include "Graphics/Camera/EntityTrackingCamera.h"
#include "Entity/Entity.h"
#include "Level/Level.h"
#include "UI/Hud.h"
#include <ctime>

#define TILESIZE 32

LevelGenerator::LevelGenerator() {}
LevelGenerator::~LevelGenerator() {}

std::vector<std::unique_ptr<Level>> LevelGenerator::GenerateLevels( InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera, Hud &hud )
{
  int mapsize = 4; // so real size is 4 by 4
  int roomWidth = 15;
  int roomLength = 10; // dont change these yet
  std::vector<int> map = GenerateMap(mapsize);

  std::vector<std::unique_ptr<Level>> levels;
  std::unique_ptr<Level> level(new Level);

  int playerStartX = ((mapsize/2) * roomWidth * TILESIZE) + (7 * TILESIZE);
  int playerStartY = 8*TILESIZE;
  std::unique_ptr<Entity> e = EntityFactory::CreatePlayer(playerStartX, playerStartY, m_inputManager);
  camera->SetEntity(e.get());
  hud.SetPlayer(e.get());

  
  int enemyX, enemyY;
  for (int i = 0; i < mapsize; ++i) {
    for (int j = 0; j < mapsize; ++j) {
      // for each room in map
      std::vector<SDL_Rect> room = GenerateRoom((i*roomWidth)+i, (j*roomLength)+j, roomWidth, roomLength, map[mapsize*j+i]); // hardcoded room width 15 length 10
      // Create walls for room
      for (SDL_Rect r: room) {
        for (int k = r.x; k < (r.x + r.w); ++k) {
          for (int l = r.y; l < (r.y + r.h); ++l) {
	    level->AddStaticEntity(EntityFactory::CreateBlock(k*TILESIZE, l*TILESIZE));
	  }
        }
        level->AddStaticEntity(EntityFactory::CreateCollisionBlock(r.x*TILESIZE, r.y*TILESIZE, r.w*TILESIZE, r.h*TILESIZE));
      }
      // Create enemy for room
      enemyX = (i*roomWidth*TILESIZE) + (5*TILESIZE);
      enemyY = (j*roomLength*TILESIZE) + (5*TILESIZE);
      level->AddEntity(std::move(EntityFactory::CreateFlyingEnemy(enemyX, enemyY, e.get())));
 
    }
  }
  level->AddEntity(std::move(e));

  int endEntityX = ((rand()%mapsize) * roomWidth * TILESIZE) + (7 * TILESIZE);
  int endEntityY = ((mapsize-1) * roomLength * TILESIZE) + (10 * TILESIZE);
  level->AddStaticEntity(EntityFactory::CreateEndLevelEntity(endEntityX, endEntityY));
  levels.push_back(std::move(level));

  return levels;
}

std::vector<int> LevelGenerator::GenerateMap(int size)
{
  if (size < 4) {
    // this random generation requires at least 4 by 4 rooms
    // if size smaller, return no rooms 
    std::vector<int> map(size*size,0);
    return map;
  }

  // We represent 2d map as 1d int vector
  // each integer represents certain type of room (see GenerateRooms)
  // initial value zero means no room
  std::vector<int> map(size*size, 0);

  // The topmost row middle index is the starting room
  int startingroom = size * 0 + (size/2);
  map[startingroom] = 1;
  
  srand (time(NULL));
  // iterate through map and create route down
  int r;
  bool set;
  for (int i = 0; i < size-1; ++i) {
    set = false;
    r = (rand() % (size-2)) + 1; // random number between 1 and size-2
    while (!set)
    {
      if (map[size * i + r] == 0) {
        map[size * i + r] = 2; // route down
        map[size * (i+1) + r] = 3; // catch the route down one level below
        set = true; // exit loop
      } else {
        r = (rand() % (size-2)+1); // there was already a room, try new index
      }
    }
  }
  // create the edge pieces
  for (int i = 0; i < size; ++i) {
    map[size * i + 0] = 4; // opening to right
    map[size * i + (size-1)] = 5; // opening to left
  }

  // fill rest with straights
  for (int i = 0; i < map.size(); ++i) {
    if (map[i] == 0) {
      map[i] = 1;
    }
  }

  return map;
}

std::vector<SDL_Rect> LevelGenerator::GenerateRoom(int x, int y, int w, int l, int n)
{
  // rooms always 15 width and 10 height
  // x and y is the left corner
  // n is which type of room we generate

  int room_width = w; int room_height = l;
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


