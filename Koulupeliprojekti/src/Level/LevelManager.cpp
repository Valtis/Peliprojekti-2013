#include "Level/LevelManager.h"
#include "Level/Level.h"
#include <stdexcept>
const int NO_ACTIVE_LEVEL = -1;
LevelManager::LevelManager() : m_currentLevel(NO_ACTIVE_LEVEL)
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

void LevelManager::Update(double ticksPassed)
{
  if (m_currentLevel == NO_ACTIVE_LEVEL)
  {
    return;
  }

  m_levels[m_currentLevel]->Update(ticksPassed);
}

Level *LevelManager::GetCurrentLevel()
{
  return m_levels[m_currentLevel].get();
}