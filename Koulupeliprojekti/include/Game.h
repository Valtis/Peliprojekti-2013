#pragma once

#if (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 6))
  #define override
#endif

#include "Level/LevelManager.h"
#include "UI/InputManager.h"
#include "UI/Window/WindowManager.h"
#include "UI/Hud.h"
#include "Graphics/Renderer.h"
#include "Message/MessageProcessor.h"
#include "Utility/Tick.h"
#include "Sound/SoundManager.h"

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
  Hud m_hud;
  Renderer m_renderer;
  Tick m_gameTick;
  Tick m_drawTick;
  bool m_running;

  std::unique_ptr<Camera> m_testDebugCamera;

};

