#include "Sound/Music.h"
#include <stdexcept>
#include <fstream>

#include <string>

Music::Music() : m_music(), m_currentSong(0), m_isPlaying(false)
{
  Mix_VolumeMusic(20); // todo: Move to UI/settings or something. Shouldn't really hard code volume...
}

Music::~Music()
{
  Uninitialize();
}

void Music::Initialize()
{
  std::ifstream inFile("data/music/music.dat");

  std::string line;
  while (std::getline(inFile, line))
  {
    ParseLine(line);
  }
}

void Music::ParseLine( std::string &line )
{
  if (line.empty())
  {
    return;
  }

  line = "data/music/" + line;
  Mix_Music *music = Mix_LoadMUS(line.c_str());
  if (music == nullptr)
  {
    throw std::runtime_error("Failed to open music file " + line + " - Error: " + Mix_GetError());
  }
  m_music.push_back(music);
}



void Music::Uninitialize() 
{
  if (m_music.size() > 0 && Mix_PlayingMusic())
  {
    Mix_HaltMusic();
  }

  for (auto music : m_music)
  {
    Mix_FreeMusic(music);
  }
  m_isPlaying = false;
  m_music.clear();
}

void Music::Play()
{
  if (m_music.empty())
  {
    return;
  }

  Mix_PlayMusic(m_music[m_currentSong], 0);
  m_isPlaying = true;
}

void Music::Pause()
{
  Mix_PauseMusic();
}

void Music::Resume()
{
  Mix_ResumeMusic();
}

void Music::Stop()
{
  Mix_HaltMusic();
  m_isPlaying = false;
}

void Music::Next()
{
  ++m_currentSong;
  if (m_currentSong >= static_cast<int>(m_music.size()))
  {
    m_currentSong = 0;
  }
  Play();
}

void Music::Previous()
{
  --m_currentSong;
  if (m_currentSong < 0)
  {
    m_currentSong = m_music.size() - 1;
  }
  Play();
}

bool Music::IsPlaying()
{
  return m_isPlaying;
}

bool Music::MusicFinished()
{
  return !Mix_PlayingMusic();
}

