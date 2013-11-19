#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <sstream>
#include <tuple>

#include <SDL.h>
#include <SDL_mixer.h>

class Music;
class SoundEffects;


class SoundManager
{
public:

  virtual ~SoundManager();

  static void Release();

  static SoundManager &Instance();

  void Play();
  void Pause();
  void Resume();
  void Stop();
  void Next();
  void Previous();
  
  void PlaySoundEffect(int id);
  void Update(double ticks_passed);

private:
  SoundManager(int frequency, int chunckSize);
  void UninitializeSDLAudio();
  void ShutdownMix();
  void UninitializeMix();

  void LoadSoundEffects();

  void LoadMusic();

  void InitializeOggSupport();

  void OpenMixAudio( int frequency, int chunkSize );

  void InitializeSDLSoundSubsystem();

  static SoundManager *m_instance;

  std::unique_ptr<Music> m_music;
  std::unique_ptr<SoundEffects> m_soundEffects;

};
