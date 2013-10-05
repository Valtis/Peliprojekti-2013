#pragma once
#include <vector>
#include <memory>

class Level;
class LevelManager
{
public:

  void Update();
private:

  std::vector<std::unique_ptr<Level>> m_Levels;
};