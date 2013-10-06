#include "Level/LevelManager.h"
#include "Level/Level.h"
#include <stdexcept>

LevelManager::LevelManager() : m_currentLevel(-1)
{

}

LevelManager::~LevelManager()
{

}

void LevelManager::SetCurrentLevel(int level)
{
  if (m_currentLevel < 0 || m_currentLevel >= m_levels.size())
  {
    throw std::runtime_error("Level id out of bounds in LevelManager::SetCurrentLevel");
  }

  m_currentLevel = level;
}