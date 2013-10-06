#pragma once
#include <vector>
#include <memory>

class Level;
class LevelManager
{
public:
  LevelManager();
  ~LevelManager();
  void Update(double ticks_passed);
  
  void SetCurrentLevel(int level);

private:
  int m_currentLevel;
  std::vector<std::unique_ptr<Level>> m_levels;
};