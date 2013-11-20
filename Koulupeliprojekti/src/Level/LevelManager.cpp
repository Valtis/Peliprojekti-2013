#include "Level/LevelManager.h"

const int NO_ACTIVE_LEVEL = -1;
LevelManager::LevelManager() : m_currentLevel(NO_ACTIVE_LEVEL)
{
  RegisterMessageHandler(MessageType::END_LEVEL, Priority::HIGHEST,
    [&](Message *msg)
    {
      return this->HandleEndLevelMessage(msg);
    }
  );
}

LevelManager::~LevelManager()
{

}

void LevelManager::Initialize(InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera)
{
  LevelGenerator lg;
  std::vector<std::unique_ptr<Level>> levels = lg.generateLevels(m_inputManager, camera);
  for (int i = 0; i < levels.size(); ++i) {
    AddLevel(std::move(levels[i]));
  }
  SetCurrentLevel(0);

}

void LevelManager::AddLevel(std::unique_ptr<Level> level)
{
  level->SetParent(this);
  m_levels.push_back(std::move(level));
}

void LevelManager::SetCurrentLevel(int level)
{
  if (level < 0 || level >= static_cast<int>(m_levels.size()))
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

MessageHandling LevelManager::HandleEndLevelMessage(Message *msg)
{
  // add player position reset here
  ++m_currentLevel;
  if (m_currentLevel >= m_levels.size())
  {
    --m_currentLevel;
    auto msg = MessageFactory::CreateEndGameMessage();
    SendMessage(msg.get());
  }
  return  MessageHandling::STOP_HANDLING;
}
