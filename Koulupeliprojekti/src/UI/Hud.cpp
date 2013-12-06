#include "UI/Hud.h"
#include "Entity/Entity.h"
#include "Component/HealthComponent.h"
#define HEALTH_SPRITE 400000
#define LIFE_SPRITE 400001
#define PAUSE_SPRITE 400004




Hud::Hud() : m_player(nullptr), m_isPaused(false)
{

}

Hud::~Hud()
{

}

std::vector<std::pair<int, SDL_Point>> Hud::GetTextureIdsAndPositions( int screenWidth, int screenHeight )
{


  SDL_assert_release(m_player != nullptr);
  HudElements elements;
 





  
  auto health = static_cast<HealthComponent *>(m_player->GetComponent(ComponentType::HEALTH));
  SDL_assert_release(health != nullptr);

  AddHitpoints(screenHeight, health, elements);
  AddLifebars(screenHeight, health, elements);
  AddPauseSprite(screenWidth, screenHeight, elements);


  return elements;
}

void Hud::AddLifebars( int screenHeight, HealthComponent *health, HudElements &elements )
{
  const int lifeStartPosWidth = 10;
  const int lifeStartPosHeight = screenHeight - 90;
  const int lifeWidth = 45;
  SDL_Point point;

  point.y = lifeStartPosHeight;

  for (int i = 0; i < health->GetLives(); ++i)
  {
    point.x = lifeStartPosWidth + i*lifeWidth;
    elements.push_back(std::make_pair(LIFE_SPRITE, point));
  }
}

void Hud::AddHitpoints( int screenHeight, HealthComponent *health, HudElements &elements )
{

  const int hpStartPosWidth = 10;
  const int hpStartPosHeight = screenHeight - 50;
  const int hpWidth = 55;
  SDL_Point point;

  point.y = hpStartPosHeight;

  for (int i = 0; i < health->GetHitpoints(); ++i)
  {
    point.x = hpStartPosWidth + i*hpWidth;
    elements.push_back(std::make_pair(HEALTH_SPRITE, point));
  }
}

void Hud::AddPauseSprite( int screenWidth, int screenHeight, HudElements &elements )
{
  if (!m_isPaused)
  {
    return;
  }

  SDL_Point point;
  const int PAUSE_SPRITE_WIDTH = 423;
  const int PAUSE_SPRITE_HEIGHT = 82;
  point.x = screenWidth/2 - PAUSE_SPRITE_WIDTH/2;
  point.y = screenHeight/2 - PAUSE_SPRITE_HEIGHT/2;
  elements.push_back(std::make_pair(PAUSE_SPRITE, point));
}

