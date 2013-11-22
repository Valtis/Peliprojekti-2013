#include "Utility/LoggerManager.h"
#include "Graphics/SpriteManager.h"
#include "Graphics/SpriteSheetLoader.h"
#include "Graphics/SpriteLoader.h"
#include "Graphics/Sprite.h"
#include "Graphics/TextureFactory.h"

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

void SpriteManager::CreateTiledSprite(Renderer *renderer, int baseID, int newID, int width, int height)
{
  SDL_assert_release(m_sprites.count(newID) == 0);
  SDL_assert_release(m_sprites.count(baseID) == 1);

  int newSpriteSheetID = 0;
  
  for (; newSpriteSheetID < 100000; ++newSpriteSheetID)
  {
    if (m_spriteSheets.count(newSpriteSheetID) == 0)
    {
      break;
    }
  }
  auto baseSprite = m_sprites[baseID].get(); 
  m_spriteSheets[newSpriteSheetID] = TextureFactory::CreateTiledSprite(renderer, m_spriteSheets[baseSprite->GetSpriteSheetID()], baseSprite->GetLocation(), width, height);
  
  std::unique_ptr<Sprite> newSprite(new Sprite(newID));
  newSprite->SetDrawPriority(baseSprite->GetDrawPriority());
  newSprite->SetSpriteSheetID(newSpriteSheetID);
  newSprite->SetLocation(0, 0, baseSprite->GetLocation().w*width, baseSprite->GetLocation().h*height);
  m_sprites[newID] = std::move(newSprite);
}