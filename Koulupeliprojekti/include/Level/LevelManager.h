#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include "Message/MessageProcessor.h"


class EntityTrackingCamera;
class Level;
class Hud;
class InputManager;

class LevelManager : public MessageProcessor
{
public:
  LevelManager();
  ~LevelManager();
  void Update(double ticksPassed);
  void AddLevel(std::unique_ptr<Level> level);
  Level *GetCurrentLevel();
  void SetCurrentLevel(int level);
  void Initialize(InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera, Hud &hud);

private:
  MessageHandling HandleEndLevelMessage(Message *msg);

  int m_currentLevel;
  std::vector<std::unique_ptr<Level>> m_levels;
};
