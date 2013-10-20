#include "Graphics/SpriteSheetLoader.h"
#include "Graphics/Renderer.h"
#include "Utility/StringUtility.h"
#include <fstream>

std::unordered_map<int, SDL_Texture *> SpriteSheetLoader::LoadSpriteSheets(std::string datafilePath)
{
  std::vector<std::pair<int, std::string>> spriteSheets = LoadSpriteSheetDetails(datafilePath);
  std::unordered_map<int, SDL_Texture *> textures;
  for (const auto &sheet : spriteSheets)
  {
    textures[sheet.first] = LoadSpriteSheet(sheet);
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
  return std::make_pair(std::stoi(tokens[1]), datafilePath + tokens[0]);
}

SDL_Texture *SpriteSheetLoader::LoadSpriteSheet(std::pair<int, std::string> sheet)
{
  SDL_Surface *spriteSheetSurface = SDL_LoadBMP(sheet.second.c_str());

  if (spriteSheetSurface == nullptr)
  {
    throw std::runtime_error("Failed to create surface - does the file " + sheet.second + " exist?");
  }

  SDL_SetColorKey(spriteSheetSurface, true, SDL_MapRGB(spriteSheetSurface->format, 255, 255, 255));
  SDL_Texture *texture = SDL_CreateTextureFromSurface(Renderer::Instance().GetRenderingContext(), spriteSheetSurface);
  SDL_FreeSurface(spriteSheetSurface);

  if (texture == nullptr)
  {
    throw std::runtime_error("Failed to create texture");
  }

  return texture;        
}

