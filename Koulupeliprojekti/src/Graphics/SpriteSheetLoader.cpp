#include "Graphics/SpriteSheetLoader.h"
#include "Graphics/Renderer.h"
#include "Utility/StringUtility.h"
#include "Utility/LoggerManager.h"
#include <fstream>
#include <stdexcept>

std::unordered_map<int, SDL_Texture *> SpriteSheetLoader::LoadSpriteSheets(Renderer *renderer, std::string datafilePath)
{
  std::vector<std::pair<int, std::string>> spriteSheets = LoadSpriteSheetDetails(datafilePath);
  std::unordered_map<int, SDL_Texture *> textures;
  for (const auto &sheet : spriteSheets)
  {
    if (textures.count(sheet.first) != 0)
    {
      std::stringstream ss;
      ss << sheet.first;
      LoggerManager::GetLog(GRAPHICS_LOG).AddLine(LogLevel::WARNING, "Sprite sheet with id " +
        ss.str() + " already exists - skipping"  );
      continue;
    }

    textures[sheet.first] = LoadSpriteSheet(renderer, sheet);
  }

  return textures;
}

std::vector<std::pair<int, std::string>> SpriteSheetLoader::LoadSpriteSheetDetails(std::string datafilePath)
{
  std::vector<std::pair<int, std::string>> spriteSheets;
  std::ifstream inFile(datafilePath + "spritesheets.dat");
  std::string line;
  while (std::getline(inFile, line))
  {
    spriteSheets.push_back(ParseSpriteSheetLine(line, datafilePath));
  }
  return spriteSheets;
}

std::pair<int, std::string> SpriteSheetLoader::ParseSpriteSheetLine(std::string line, std::string datafilePath)
{
  std::vector<std::string> tokens = Utility::Tokenize(line, " ");
  if (tokens.size() < 2)
  {
    throw std::runtime_error("Malformed sprite sheet data file: Line " + line + " encountered");
  }
  int tokenV;
  std::istringstream(tokens[1]) >> tokenV;
  return std::make_pair(tokenV, datafilePath + tokens[0]);
}

SDL_Texture *SpriteSheetLoader::LoadSpriteSheet(Renderer *renderer, std::pair<int, std::string> sheet)
{
  SDL_Surface *spriteSheetSurface = SDL_LoadBMP(sheet.second.c_str());

  if (spriteSheetSurface == nullptr)
  {
    throw std::runtime_error("Failed to create surface - does the file " + sheet.second + " exist?");
  }

  SDL_SetColorKey(spriteSheetSurface, true, SDL_MapRGB(spriteSheetSurface->format, 255, 255, 255));
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer->GetRenderingContext(), spriteSheetSurface);
  SDL_FreeSurface(spriteSheetSurface);

  if (texture == nullptr)
  {
    throw std::runtime_error("Failed to create texture");
  }

  return texture;
}

