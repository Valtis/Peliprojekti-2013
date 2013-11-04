#include "Graphics/SpriteLoader.h"
#include "Graphics/Sprite.h"
#include <stdexcept>

std::vector<std::unique_ptr<Sprite>> SpriteLoader::LoadSprites(std::string datafilePath)
{
  std::vector<SpriteDataFileLine> spriteData = LoadSpriteDetails(datafilePath);

  std::vector<std::unique_ptr<Sprite>> sprites;
  for (auto &helper : spriteData)
  {
    sprites.push_back(std::move(CreateSprite(helper)));
  }
  return sprites;
}


std::vector<SpriteDataFileLine> SpriteLoader::LoadSpriteDetails(std::string datafilePath)
{
  std::vector<SpriteDataFileLine> spriteData;

  std::ifstream inFile(datafilePath + "sprites.dat");
  std::string line;
  while (std::getline(inFile, line))
  {

    spriteData.push_back(ParseSpriteLine(line, datafilePath));
  }
  return spriteData;
}

SpriteDataFileLine SpriteLoader::ParseSpriteLine(std::string line, std::string datafilePath)
{
  std::vector<std::string> tokens = Utility::Tokenize(line, " ");
  if (tokens.size() < 6)
  {
    throw std::runtime_error("Malformed sprite data file: Line " + line + " encountered");
  }

  SpriteDataFileLine dataLine;
  dataLine.x = std::stoi(tokens[0]);
  dataLine.y = std::stoi(tokens[1]);
  dataLine.width = std::stoi(tokens[2]);
  dataLine.height = std::stoi(tokens[3]);
  dataLine.spriteSheetID = std::stoi(tokens[4]);
  dataLine.spriteID = std::stoi(tokens[5]);
  if (tokens.size() >= 7)
  {

    dataLine.drawPriority = std::stoi(tokens[6]);
  }
  else
  {
    dataLine.drawPriority = 5;
  }
  return dataLine;
}

std::unique_ptr<Sprite> SpriteLoader::CreateSprite(SpriteDataFileLine dataLine)
{
  std::unique_ptr<Sprite> sprite(new Sprite(dataLine.spriteID));
  sprite->SetSpriteSheetID(dataLine.spriteSheetID);
  sprite->SetLocation(dataLine.x, dataLine.y, dataLine.width, dataLine.height);
  sprite->SetDrawPriority(dataLine.drawPriority);
  return sprite;
}
