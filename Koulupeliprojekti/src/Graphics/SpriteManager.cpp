#include "Utility/LoggerManager.h"
#include "Graphics/SpriteManager.h"
#include "Graphics/SpriteSheetLoader.h"
#include "Graphics/SpriteLoader.h"
#include "Graphics/Sprite.h"

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

void SpriteManager::Initialize(Renderer *renderer, std::string datafilePath)
{
  LoadSpriteSheets(renderer, datafilePath);
  LoadSprites(datafilePath);
}

void SpriteManager::LoadSpriteSheets(Renderer *renderer, std::string datafilePath)
{
  SpriteSheetLoader loader;
  for (auto spritesheet : m_spriteSheets)
  {
    SDL_DestroyTexture(spritesheet.second);
  }

  m_spriteSheets = loader.LoadSpriteSheets(renderer, datafilePath);
}

void SpriteManager::LoadSprites(std::string datafilePath)
{
  SpriteLoader loader;
  std::vector<std::unique_ptr<Sprite>> sprites = loader.LoadSprites(datafilePath);
  
  for (auto &sprite : sprites)
  {
    if (m_sprites.count(sprite->GetID()) != 0)
    {
      std::stringstream ss;
      ss << (int)(sprite->GetID());
      LoggerManager::GetLog(GRAPHICS_LOG).AddLine(LogLevel::WARNING, "Sprite with id " +
        ss.str() + " already exists - skipping");
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