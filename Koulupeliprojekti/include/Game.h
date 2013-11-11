#pragma once

#include "UI/InputManager.h"
#include "Level/LevelManager.h"
#include "UI/Window/WindowManager.h"
#include "Graphics/Renderer.h"
#include "Message/MessageProcessor.h"
#include "Utility/Tick.h"

class Camera;
class Game : public MessageProcessor
{
public:
  Game();
  ~Game();

  void Run();

private:
  void Initialize();

  void TestWindowCreation();

  void UpdateGameState();
  void Draw();
  void HandleInput();

  void ShutDownGame();


  LevelManager m_levelManager;
  InputManager m_inputManager;
  WindowManager m_windowManager;
  Renderer m_renderer;
  Tick m_gameTick;
  Tick m_drawTick;
  bool m_running;

  std::unique_ptr<Camera> m_testDebugCamera;

};

