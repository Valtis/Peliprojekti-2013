#pragma once

#include "Level/LevelManager.h"
#include "UI/InputManager.h"
#include "UI/Window/WindowManager.h"
#include "UI/Hud.h"
#include "Graphics/Renderer.h"
#include "Message/MessageProcessor.h"
#include "Utility/Tick.h"
#include "Sound/SoundManager.h"

class Camera;
class Command;
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
  void PollEvents();
  MessageHandling HandleInput(Command *cmd);

  void ShutDownGame();


  LevelManager m_levelManager;
  InputManager m_inputManager;
  WindowManager m_windowManager;
  SoundManager m_soundManager;
  Hud m_hud;
  Renderer m_renderer;
  Tick m_gameTick;
  Tick m_drawTick;
  bool m_isRunning;
  bool m_isPaused;
  bool m_playerAlive;
  std::unique_ptr<Camera> m_camera;

};

