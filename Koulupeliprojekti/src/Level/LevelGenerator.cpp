#include "Level/LevelGenerator.h"
#include "Entity/EntityFactory.h"
#include "Graphics/Camera/EntityTrackingCamera.h"
#include "Entity/Entity.h"
#include "Level/Level.h"
#include "UI/Hud.h"
#include <ctime>

#define TILESIZE 70
#define ROOMWIDTH 10
#define ROOMLENGTH 12

LevelGenerator::LevelGenerator() {}
LevelGenerator::~LevelGenerator() {}

std::vector<std::unique_ptr<Level>> LevelGenerator::GenerateLevels( InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera, Hud &hud )
{
  int mapsize = 6; // so real size is 6 by 6
  std::vector<int> map = GenerateMap(mapsize);

  std::vector<std::unique_ptr<Level>> levels;
  std::unique_ptr<Level> level(new Level);

  int playerStartX = ((mapsize/2) * ROOMWIDTH * TILESIZE) + (7 * TILESIZE);
  int playerStartY = 8*TILESIZE;
  std::unique_ptr<Entity> e = EntityFactory::CreatePlayer(playerStartX, playerStartY, m_inputManager);
  level->SetLevelStartPoint(std::make_pair(playerStartX, playerStartY));
  camera->SetEntity(e.get());
  hud.SetPlayer(e.get());

  
  int enemyX, enemyY;
  for (int i = 0; i < mapsize; ++i) {
    for (int j = 0; j < mapsize; ++j) {
      // for each room in map
      std::vector<SDL_Rect> room = GenerateRoom((i*ROOMWIDTH)+i, (j*ROOMLENGTH)+j, ROOMWIDTH, ROOMLENGTH, map[mapsize*j+i]); // hardcoded room width 15 length 10
      // Create walls for room
      for (SDL_Rect r: room) {
        for (int k = r.x; k < (r.x + r.w); ++k) {
          for (int l = r.y; l < (r.y + r.h); ++l) {
	    level->AddStaticEntity(EntityFactory::CreateBlock(k*TILESIZE, l*TILESIZE, 400005));
	  }
        }
        level->AddStaticEntity(EntityFactory::CreateCollisionBlock(r.x*TILESIZE, r.y*TILESIZE, r.w*TILESIZE, r.h*TILESIZE));
      }
      CreateBackgroundElements((i*ROOMWIDTH)+i, (j*ROOMLENGTH)+j, map[mapsize*j+i], level);
      // Create enemy for room
      enemyX = (i*ROOMWIDTH*TILESIZE) + (5*TILESIZE);
      enemyY = (j*ROOMLENGTH*TILESIZE) + (5*TILESIZE);
      level->AddEntity(std::move(EntityFactory::CreateFlyingEnemy(enemyX, enemyY, e.get())));
 
    }
  }
  level->AddEntity(std::move(e));

  int endEntityX = ((rand()%mapsize) * ROOMWIDTH * TILESIZE) + (7 * TILESIZE);
  int endEntityY = ((mapsize-1) * ROOMLENGTH * TILESIZE) + ((ROOMLENGTH-1) * TILESIZE);
  level->AddEntity(EntityFactory::CreateEndLevelEntity(endEntityX, endEntityY));
  levels.push_back(std::move(level));

  return levels;
}

void LevelGenerator::CreateBackgroundElements(int x, int y, int roomtype, std::unique_ptr<Level>& level)
{
  // box 400007, sign to right 400008, sign to left 400009

  int boxX = (x + (ROOMWIDTH/2)) * TILESIZE + 2*(TILESIZE);
  int boxY = (y + (ROOMLENGTH-1)) * TILESIZE;
  level->AddStaticEntity(EntityFactory::CreateBlock(boxX, boxY, 400007));

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

  std::vector<SDL_Rect> room(0);
  SDL_Rect p1, p2, p3, p4, p5, p6;
  SDL_Rect left_wall;
  left_wall.x = x, left_wall.y = y+1, left_wall.h = ROOMLENGTH - 1, left_wall.w = 1;
  SDL_Rect right_wall;
  right_wall.x = x + ROOMWIDTH, right_wall.y = y + 1, right_wall.h = ROOMLENGTH - 1, right_wall.w = 1;
  SDL_Rect top_wall;
  top_wall.x = x, top_wall.y = y, top_wall.h = 1, top_wall.w = ROOMWIDTH+1;
  SDL_Rect bottom_wall;
  bottom_wall.x = x, bottom_wall.y = y + ROOMLENGTH, bottom_wall.h = 1, bottom_wall.w = ROOMWIDTH+1;
  switch (n)
  {
    case 1: // room has opening to left and right
      right_wall.h = right_wall.h - 2;
      left_wall.h = left_wall.h - 2;
      room.push_back(left_wall), room.push_back(right_wall), room.push_back(top_wall), room.push_back(bottom_wall);
      break;
    case 2: // room has opening left, right and down
      right_wall.h = right_wall.h - 2;
      left_wall.h = left_wall.h - 2;
      room.push_back(left_wall), room.push_back(right_wall), room.push_back(top_wall);
      // we split the floor
      bottom_wall.w = ((ROOMWIDTH+1) / 2 ) - 2;
      p1.x = x + (ROOMWIDTH+1)/2;
      p1.y = bottom_wall.y, p1.h = 1, p1.w = (ROOMWIDTH+1) - (bottom_wall.w + 2);
      room.push_back(bottom_wall), room.push_back(p1);
      break;
    case 3: // opening left, right and up
      right_wall.h = right_wall.h - 2;
      left_wall.h = left_wall.h - 2;
      room.push_back(left_wall), room.push_back(right_wall), room.push_back(bottom_wall);
      // we split the top
      top_wall.w = ((ROOMWIDTH+1) / 2) -2;
      p1.x = x + (ROOMWIDTH+1)/2;
      p1.y = top_wall.y, p1.h = 1, p1.w = (ROOMWIDTH+1) - (top_wall.w + 2);
      room.push_back(top_wall), room.push_back(p1);
      break;
    case 4: // opening to right
      room.push_back(top_wall), room.push_back(left_wall), room.push_back(bottom_wall);
      break;
    case 5: // opening to left
      room.push_back(top_wall), room.push_back(right_wall), room.push_back(bottom_wall);
      break;
    default: // room 6 opening to every direction
      right_wall.h = right_wall.h - 2;
      left_wall.h = left_wall.h - 2;
      top_wall.w = ((ROOMWIDTH+1) / 2) -2;
      p1.x = x + (ROOMWIDTH+1)/2;
      p1.y = top_wall.y, p1.h = 1, p1.w = (ROOMWIDTH+1) - (top_wall.w + 2);
      bottom_wall.w = ((ROOMWIDTH+1) / 2 ) - 2;
      p2.x = x + (ROOMWIDTH+1)/2;
      p2.y = bottom_wall.y, p2.h = 1, p2.w = (ROOMWIDTH+1) - (bottom_wall.w + 2);
      room.push_back(bottom_wall), room.push_back(right_wall), room.push_back(left_wall);
      room.push_back(top_wall), room.push_back(p1), room.push_back(p2);
      break;
  }
  return room;
}


