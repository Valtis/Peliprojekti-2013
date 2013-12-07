#include "Sound/SoundEffects.h"
#include "Utility/StringUtility.h"
#include "Utility/LoggerManager.h"
#include <fstream>
#include <stdexcept>
#include <sstream>

int StrToInt(std::string const str)
{
  std::stringstream ss;
  ss.str(str);
  int i = 0;
  ss >> i;
  return i;
}

std::string IntToStr(int i)
{
  std::stringstream ss;
  ss.str("");
  ss << i;
  return ss.str();
}

SoundEffects::SoundEffects() : m_effects()
{

}

SoundEffects::~SoundEffects()
{
  Uninitialize();
}

void SoundEffects::Initialize()
{

  std::ifstream inFile("data/sound/soundeffects.dat");
  std::string line;
  std::getline(inFile, line);
  Mix_Volume(-1, StrToInt(line));

  while (std::getline(inFile, line))
  {
    ParseLine(line);
  }
 
}

void SoundEffects::ParseLine( std::string &line )
{
  std::vector<std::string > tokens = Utility::Tokenize(line, " ");

  if (tokens.size() < 2)
  {
    return;
  }

  int i = StrToInt(tokens[1]);

  if (m_effects.count(i) != 0)
  {
    LoggerManager::GetLog(SOUND_LOG).AddLine(LogLevel::WARNING, "Sound effect with id " + tokens[1] + " already exists - skipping");
    return;
  }

  LoadEffect("data/sound/" + tokens[0], i);
}


void SoundEffects::LoadEffect( std::string name, int id )
{
  Mix_Chunk *effect = Mix_LoadWAV(name.c_str());

  if (effect == nullptr)
  {
    throw std::runtime_error("Failed to load sound effect " + name);
  }
  m_effects[id] = effect;
}



void SoundEffects::Uninitialize()
{
  for (auto effect : m_effects)
  {
    if (effect.second != nullptr)
    {
      Mix_FreeChunk(effect.second);
    }
  }

  m_effects.clear();
}

void SoundEffects::Play(int id)
{
  if (m_effects.count(id) == 0)
  {
    LoggerManager::GetLog(SOUND_LOG).AddLine(LogLevel::WARNING,
        "Sound effect with id " + IntToStr(id) + " does not exist - skipping");
    return;
  }
  Mix_PlayChannel(-1, m_effects[id], 0);
}

