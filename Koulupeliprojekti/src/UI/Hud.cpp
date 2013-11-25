#include "UI/Hud.h"
#include "Entity/Entity.h"
#include "Component/HealthComponent.h"
#define HEALTH_SPRITE 400000
#define LIFE_SPRITE 400001


typedef std::vector<std::pair<int, SDL_Point>> HudElements;

Hud::Hud()
{

}

Hud::~Hud()
{

}

std::vector<std::pair<int, SDL_Point>> Hud::GetTextureIdsAndPositions( int screenWidth, int screenHeight )
{
  SDL_assert_release(m_player != nullptr);
  HudElements elements;
  SDL_Point point;

  const int hpStartPosWidth = 10;
  const int hpStartPosHeight = screenHeight - 30;
  const int hpWidth = 25;

  const int lifeStartPosWidth = 10;
  const int lifeStartPosHeight = screenHeight - 60;
  const int lifeWidth = 25;

  
  auto health = static_cast<HealthComponent *>(m_player->GetComponent(ComponentType::HEALTH));
  SDL_assert_release(health != nullptr);

  point.y = hpStartPosHeight;

  for (int i = 0; i < health->GetHitpoints(); ++i)
  {
    point.x = hpStartPosWidth + i*hpWidth;
    elements.push_back(std::make_pair(HEALTH_SPRITE, point));
  }

   point.y = lifeStartPosHeight;

  for (int i = 0; i < health->GetLives(); ++i)
  {
    point.x = lifeStartPosWidth + i*lifeWidth;
    elements.push_back(std::make_pair(LIFE_SPRITE, point));
  }

  return elements;
}

