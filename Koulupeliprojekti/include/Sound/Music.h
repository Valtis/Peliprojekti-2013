#pragma once



#include <vector>
#include <SDL_mixer.h>
#include <string>	

class Music
{
public:
  Music();
  virtual ~Music();
  void Initialize();

  void ParseLine( std::string &line );

  void Uninitialize();

  void Play();
  void Pause();
  void Resume();
  void Stop();

  void Next();
  void Previous();
  bool IsPlaying();
  bool MusicFinished();

protected:
private:
  std::vector<Mix_Music *> m_music;
  int m_currentSong;
  bool m_isPlaying;
};


