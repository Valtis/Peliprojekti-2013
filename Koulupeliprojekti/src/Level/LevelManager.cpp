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


void LevelManager::AddLevel(std::unique_ptr<Level> level)
{
  m_levels.push_back(std::move(level));
}

void LevelManager::SetCurrentLevel(int level)
{
  if (level < 0 || level >= m_levels.size())
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