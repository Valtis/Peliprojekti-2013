#include "Utility/LoggerManager.h"
#include "Graphics/SpriteManager.h"
#include "Graphics/SpriteSheetLoader.h"
#include "Graphics/SpriteLoader.h"


SpriteManager::SpriteManager()
{

}

SpriteManager::~SpriteManager()
{
  for (auto i : m_spriteSheets)
  {
    SDL_DestroyTexture(i.second);
  }

  m_spriteSheets.clear();
}

void SpriteManager::Initialize(std::string datafilePath)
{
  LoadSpriteSheets(datafilePath);
  LoadSprites(datafilePath);
}

void SpriteManager::LoadSpriteSheets(std::string datafilePath)
{
  SpriteSheetLoader loader;
  m_spriteSheets = loader.LoadSpriteSheets(datafilePath);
}

void SpriteManager::LoadSprites(std::string datafilePath)
{
  SpriteLoader loader;
  std::vector<std::unique_ptr<Sprite>> sprites = loader.LoadSprites(datafilePath);
  
  for (auto &sprite : sprites)
  {
    if (m_sprites.count(sprite->GetID()) != 0)
    {
      LoggerManager::GetLog(GRAPHICS_LOG).AddLine(LogLevel::WARNING, "Sprite with id " + std::to_string(sprite->GetID()) + " already exists - skipping");
      continue;
    }
    m_sprites[sprite->GetID()] = std::move(sprite);
  }
}


Sprite *SpriteManager::GetSprite(int id)
{
  if (m_sprites.count(id) == 0)
  {
    return nullptr;
  }

  return m_sprites[id].get();
}

SDL_Texture *SpriteManager::GetSpriteSheet(int spriteSheetID)
{
  if (m_spriteSheets.count(spriteSheetID) == 0)
  {
    return nullptr;
  }

  return m_spriteSheets[spriteSheetID];
}