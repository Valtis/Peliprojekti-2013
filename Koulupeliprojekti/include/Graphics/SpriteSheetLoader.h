#pragma once
#include <unordered_map>
#include <SDL.h>
#include <vector>
#include <string>
class SpriteSheetLoader
{
public:
  SpriteSheetLoader() { }
  ~SpriteSheetLoader() { }
  std::unordered_map<int, SDL_Texture *> LoadSpriteSheets(std::string datafilePath);

private:
  std::vector<std::pair<int, std::string>> LoadSpriteSheetDetails(std::string datafilePath);
  std::pair<int, std::string> ParseSpriteSheetLine(std::string line, std::string datafilePath);
  SDL_Texture *LoadSpriteSheet(std::pair<int, std::string> sheet);
};