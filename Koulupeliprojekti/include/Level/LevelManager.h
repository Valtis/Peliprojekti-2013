#pragma once
#include <vector>
#include <memory>
#include "Graphics/Camera/EntityTrackingCamera.h"
#include "UI/InputManager.h"
#include "Message/MessageProcessor.h"
#include "Component/AiComponent.h"
#include "Component/InputComponent.h"

class Level;
class LevelManager : public MessageProcessor
{
public:
  LevelManager();
  ~LevelManager();
  void Update(double ticksPassed);
  void AddLevel(std::unique_ptr<Level> level);
  Level *GetCurrentLevel();
  void SetCurrentLevel(int level);
  void Initialize(InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera);
  void CreateEndLevelEntity(int frame, int x, int y, int size, std::unique_ptr<Level>& level);
  void CreateBlock(int frame, int x, int y, int size, std::unique_ptr<Level>& level, bool hitbox);
  void CreateEnemy(int frame, int x, int y, int size, std::unique_ptr<Level>& level,
                               std::unique_ptr<AiComponent>& ai, std::unique_ptr<InputComponent>& ci);
  std::unique_ptr<Entity> CreatePlayer(int frame, int x, int y, int size, std::unique_ptr<InputComponent>& ci);

private:
  MessageHandling HandleEndLevelMessage(Message *msg);

  int m_currentLevel;
  std::vector<std::unique_ptr<Level>> m_levels;
};
