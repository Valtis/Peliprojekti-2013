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
class MessageProcessor;
enum class MessageHandling;
class Message;
class SoundManager
{
public:
  SoundManager(int frequency, int chunckSize);
  virtual ~SoundManager();
  void RegisterMessageHandlers(MessageProcessor *processor);
 
  void Play();
  void Pause();
  void Resume();
  void Stop();
  void Next();
  void Previous();
  
  void PlaySoundEffect(int id);
  void Update(double ticks_passed);

private:
  MessageHandling HandlePlaySoundEffectMessage(Message *msg);
  void UninitializeSDLAudio();
  void ShutdownMix();
  void UninitializeMix();

  void LoadSoundEffects();

  void LoadMusic();

  void InitializeOggSupport();

  void OpenMixAudio( int frequency, int chunkSize );

  void InitializeSDLSoundSubsystem();

  std::unique_ptr<Music> m_music;
  std::unique_ptr<SoundEffects> m_soundEffects;

};
