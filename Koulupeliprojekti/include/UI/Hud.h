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

  std::vector<std::pair<int, SDL_Rect>> GetTextures();

private:
  enum class HudTextures : int { NONE, HEALTH_COUNTER, LIFE_COUNTER };
  std::map<HudTextures, int> m_textures;
  Entity *m_player;

};