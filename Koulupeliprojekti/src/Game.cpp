#include "Game.h"
#include "Level/Level.h"
#include "Utility/LoggerManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/Camera/EntityTrackingCamera.h"
#include "Component/WalkingAiComponent.h"

// for testing purposes - testing windows -  feel free to remove
#include "UI/Window/Window.h"
#include "UI/Window/Button.h"
#include "UI/Window/TextBox.h"
#include "Message/Commands/ControlCommand.h"

Game::Game() : m_gameTick(30), m_drawTick(30), m_isRunning(false), m_isPaused(false)
{
  RegisterMessageHandler(MessageType::END_GAME, Priority::HIGHEST, [&](Message *msg)
    {
      this->m_windowManager.GetWindows()[1]->Activate();
      return MessageHandling::STOP_HANDLING;
    }
  );

}


Game::~Game()
{
}


void Game::Run()
{
  Initialize();

  while (m_isRunning)
  {
    UpdateGameState();
    Draw();
  }
}

void Game::UpdateGameState()
{
  if (m_gameTick.TickHasPassed())
  {
    SoundManager::Instance().Update(m_gameTick.TicksPassed());
    PollEvents();

    if (!m_isPaused) 
    {
      m_levelManager.Update(m_gameTick.TicksPassed());
    }
    
  }
}

void Game::Draw()
{
  if (m_drawTick.TickHasPassed())
  {
    m_renderer.Draw(
      m_testDebugCamera.get(), m_levelManager.GetCurrentLevel()->GetEntities(),
      m_levelManager.GetCurrentLevel()->GetStaticEntities(),
      m_levelManager.GetCurrentLevel()->GetStaticCollidables(),
      m_windowManager.GetWindows(),
      m_hud
      );
  }
}

void Game::Initialize()
{
  LoggerManager::SetGlobalLogLevel(LogLevel::ALL);
  LoggerManager::SetLogFolder("logs");
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  SDL_JoystickEventState(SDL_ENABLE);

  SoundManager::Instance().Play();

  // test code - lots of stuff hard coded
  m_renderer.RegisterMessageHandlers(this);
  m_renderer.CreateWindow("Title", 800, 600);
  m_renderer.LoadSprites("data/sprites/");


  FontManager::Instance().Initialize("data/fonts/FreeMono.otf");
  m_inputManager.Initialize();
  m_inputManager.RegisterInputHandler([&](Command* cmd) { return m_windowManager.HandleInput(cmd); }, 10);
  m_inputManager.RegisterInputHandler([&](Command *cmd) { return this->HandleInput(cmd); } , 10);

  std::unique_ptr<EntityTrackingCamera> camera(new EntityTrackingCamera);

  m_levelManager.SetParent(this);
  m_levelManager.Initialize(m_inputManager, camera, m_hud);
  
  m_testDebugCamera = std::move(camera);

  TestWindowCreation();
  m_isRunning = true;

}

MessageHandling Game::HandleInput( Command *cmd )
{
  if (cmd->GetType() != MessageType::CONTROL_COMMAND )
  {
    return MessageHandling::PASS_FORWARD;
  }

  auto controlCmd = static_cast<ControlCommand *>(cmd);
  if (controlCmd->GetCommand() == Action::PAUSE && controlCmd->GetState() == KeyState::DOWN)
  {
    m_isPaused = !m_isPaused;
    m_hud.SetPauseStatus(m_isPaused);
  }

  if (m_isPaused)
  {
    return MessageHandling::STOP_HANDLING;
  }
  return MessageHandling::PASS_FORWARD;
}


void Game::PollEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
     if (event.type == SDL_QUIT)
     {
       m_isRunning = false;
       return;
     }
     m_inputManager.HandleInput(event);
  }
}

// for testing only - can be removed!
void Game::ShutDownGame()
{
  m_isRunning = false;
}

void Game::TestWindowCreation()
{
  SDL_Rect location;
  SDL_Color color;

  location.x = 20;
  location.y = 20;
  location.w = 230;
  location.h = 300;

  color.r = 201;
  color.g = 240;
  color.b = 248;
  color.a = 255;


  std::unique_ptr<Window> window(new Window(location, color, &m_renderer));
  window->Activate();
  location.x = 10;
  location.y = 250;
  location.w = 140;
  location.h = 30;
  std::unique_ptr<Button> button(new Button(location, "Close window", &m_renderer));

  button->AddHandler([&]{ this->m_windowManager.GetWindows()[0]->Deactivate(); });
  window->AddWindow(std::move(button));

  location.x = 10;
  location.y = 10;
  location.w = 200;
  location.h = 230;
  std::unique_ptr<TextBox> textBox(new TextBox("THIS IS UNFINISHED. Just pretend we have fixed the issues with collision detection, that the triangle is animated player and shooting actually does something", location, &m_renderer));
  window->AddWindow(std::move(textBox));

  m_windowManager.AddWindow(std::move(window));


  location.x = 300;
  location.y = 300;
  location.w = 300;
  location.h = 300;

  window.reset(new Window(location, color, &m_renderer));
  window->Deactivate();

  location.x = 10;
  location.y = 10;
  location.w = 280;
  location.h = 240;
  textBox.reset(new TextBox("Congratulations! You have finished the demo version of this game!", location, &m_renderer));
  window->AddWindow(std::move(textBox));


  location.x = 10;
  location.y = 260;
  location.w = 140;
  location.h = 30;

  button.reset(new Button(location, "Close game", &m_renderer));

  button->AddHandler([&]{ this->ShutDownGame(); });
  window->AddWindow(std::move(button));
  m_windowManager.AddWindow(std::move(window));

}

