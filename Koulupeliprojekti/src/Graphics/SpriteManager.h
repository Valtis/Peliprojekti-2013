#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <SDL.h>

class Sprite;
class Renderer;
class SpriteManager
{
public:
  SpriteManager();
  ~SpriteManager();


  void Initialize(Renderer *renderer, std::string datafilePath);

  Sprite *GetSprite(int id);
  SDL_Texture *GetSpriteSheet(int spriteSheetID);

  void CreateTiledSprite(Renderer *renderer, int baseID, int newID, int width, int height);

private:

  void LoadSpriteSheets(Renderer *renderer, std::string datafilePath);
  void LoadSprites(std::string datafilePath);

  std::unordered_map<int, std::unique_ptr<Sprite>> m_sprites;
  std::unordered_map<int, SDL_Texture *> m_spriteSheets;
};