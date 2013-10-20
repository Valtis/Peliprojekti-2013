#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <SDL.h>

#include "Graphics/Sprite.h"

class SpriteManager
{
public:
  SpriteManager();
  ~SpriteManager();


  void Initialize(std::string datafilePath);

  Sprite *GetSprite(int id);

  SDL_Texture *GetSpriteSheet(int spriteSheetID);


private:

  void LoadSpriteSheets(std::string datafilePath);
  void LoadSprites(std::string datafilePath);

  void CreateSprite(SpriteDataFileLine helper);

  std::unordered_map<int, std::unique_ptr<Sprite>> m_sprites;
  std::unordered_map<int, SDL_Texture *> m_spriteSheets;
};