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

  void SetPlayer(Entity *player) { m_player = player; }
  std::vector<std::pair<int, SDL_Point>> GetTextureIdsAndPositions(int screenWidth, int screenHeight);

private:
  Entity *m_player;
};