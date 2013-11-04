#pragma once
#include <vector>
#include <memory>
#include "Graphics/Camera/EntityTrackingCamera.h"
#include "UI/InputManager.h"

class Level;
class LevelManager
{
public:
  LevelManager();
  ~LevelManager();
  void Update(double ticksPassed);
  void AddLevel(std::unique_ptr<Level> level);
  Level *GetCurrentLevel();
  void SetCurrentLevel(int level);
  void Initialize(InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera);

private:
  int m_currentLevel;
  std::vector<std::unique_ptr<Level>> m_levels;
};
