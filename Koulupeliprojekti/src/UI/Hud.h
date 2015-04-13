#pragma once
#include <vector>
#include <map>
#include <SDL.h>
class Entity;
class HealthComponent;

class Hud
{
public:
  typedef std::vector<std::pair<int, SDL_Point>> HudElements;
  Hud();
  ~Hud();

  void SetPauseStatus(bool pauseStatus) { m_isPaused = pauseStatus; }
  void SetPlayer(Entity *player) { m_player = player; }
  std::vector<std::pair<int, SDL_Point>> GetTextureIdsAndPositions(int screenWidth, int screenHeight);
 
  void AddPauseSprite( int screenWidth, int screenHeight, HudElements &elements );

private:

  void AddHitpoints( int screenHeight, HealthComponent *health, HudElements &elements );

  void AddLifebars( int screenHeight, HealthComponent *health, HudElements &elements );

  Entity *m_player;
  bool m_isPaused;
};