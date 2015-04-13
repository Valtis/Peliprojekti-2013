#pragma once
#include <unordered_map>
#include <SDL.h>
#include <vector>
#include <string>
class Renderer;
class SpriteSheetLoader
{
public:
  SpriteSheetLoader() { }
  ~SpriteSheetLoader() { }
  std::unordered_map<int, SDL_Texture *> LoadSpriteSheets(Renderer *renderer, std::string datafilePath);

private:
  std::vector<std::pair<int, std::string>> LoadSpriteSheetDetails(std::string datafilePath);
  std::pair<int, std::string> ParseSpriteSheetLine(std::string line, std::string datafilePath);
  SDL_Texture *LoadSpriteSheet(Renderer *renderer, std::pair<int, std::string> sheet);
};