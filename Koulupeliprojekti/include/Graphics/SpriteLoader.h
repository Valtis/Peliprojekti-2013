#pragma once
#include <vector>
#include <string>
#include "Utility/StringUtility.h"
#include <fstream>
#include <memory>

struct SpriteDataFileLine
{
  int x;
  int y;
  int width;
  int height;
  int spriteSheetID;
  int spriteID;
  int drawPriority;
};


class SpriteLoader
{
public:
  SpriteLoader() { }
  ~SpriteLoader() { }
  std::vector<std::unique_ptr<Sprite>> SpriteLoader::LoadSprites(std::string datafilePath);

private:

  std::vector<SpriteDataFileLine> LoadSpriteDetails(std::string datafilePath);
  SpriteDataFileLine ParseSpriteLine(std::string line, std::string datafilePath);
  std::unique_ptr<Sprite> CreateSprite(SpriteDataFileLine dataLine);
};