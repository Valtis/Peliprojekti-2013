#pragma once
#include <SDL_mixer.h>
#include <unordered_map>
#include <string>

class SoundEffects
{
public:
  SoundEffects();
  virtual ~SoundEffects();

  void Initialize();
  void ParseLine( std::string &line );
  void Uninitialize();
  void Play(int id);

protected:
private:

  void LoadEffect( std::string name, int id );
  std::unordered_map<int, Mix_Chunk *> m_effects;

};
