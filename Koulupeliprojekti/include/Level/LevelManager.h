#pragma once
#include <vector>
#include <memory>

class Level;
class LevelManager
{
public:
  LevelManager();
  ~LevelManager();
  void Update(double ticksPassed);
  
  void SetCurrentLevel(int level);

private:
  int m_currentLevel;
  std::vector<std::unique_ptr<Level>> m_levels;
};