#pragma once

#include "UI/InputManager.h"
#include "Level/LevelManager.h"
#include "Utility/Tick.h"
class Game
{
public:
  Game();
  ~Game();

  void Run();

private:
  void Initialize();
  void RunMainLoop();
  void UpdateGameState();
  void Draw();
  void HandleInput();

  LevelManager m_levelManager;
  Tick m_gameTick;
  Tick m_drawTick;
  InputManager m_inputManager;
};

