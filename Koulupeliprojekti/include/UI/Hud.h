#pragma once
#include <vector>
#include <map>
#include <SDL.h>
class Entity;
class Hud
{
public:
  Hud();
  ~Hud();

  std::vector<std::pair<int, SDL_Point>> GetTextureIdsANdPositions(int screenWidth, int screenHeight);

private:
  Entity *m_player;
};