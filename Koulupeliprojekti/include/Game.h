#pragma once

#include "UI/InputManager.h"
#include "Level/LevelManager.h"
#include "UI/Window/WindowManager.h"
#include "Graphics/Renderer.h"
#include "UI/Window/ButtonHandler.h"
#include "Utility/Tick.h"


class Game : public ButtonHandler
{
public:
  Game();
  ~Game();

  void Run();

private:
  void Initialize();

  void UpdateGameState();
  void Draw();
  void HandleInput();

  void OnButtonClick() override;


  LevelManager m_levelManager;
  InputManager m_inputManager;
  WindowManager m_windowManager;
  Renderer m_renderer;
  Tick m_gameTick;
  Tick m_drawTick;
  bool m_running;
 
};

