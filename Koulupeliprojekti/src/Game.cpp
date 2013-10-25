#include "Game.h"
#include "Level/Level.h"
#include "Utility/LoggerManager.h"
#include "Graphics/FontManager.h"
// for testing purposes until level manager actually contains levels - feel free to remove
#include "Entity/Entity.h"
// for testing purposes - testing windows -  feel free to remove
#include "UI/Window/Window.h"
#include "UI/Window/Button.h"
#include "Message/Commands/MouseCommand.h"

Game::Game() : m_gameTick(30), m_drawTick(30)
{
}


Game::~Game()
{
}


void Game::Run()
{
  Initialize();

  while (m_running)
  {
    UpdateGameState();
    Draw();
  }
}

void Game::UpdateGameState()
{
  if (m_gameTick.TickHasPassed())
  {
    HandleInput();
    m_levelManager.Update(m_gameTick.TicksPassed());
  }
}

void Game::Draw()
{
  if (m_drawTick.TickHasPassed())
  {
    //     m_renderer.Draw(nullptr, m_levelManager.GetCurrentLevel()->GetEntities(), m_windowManager.GetWindows());
    std::vector<std::unique_ptr<Entity>> e;
    m_renderer.Draw(nullptr, e, m_windowManager.GetWindows());
  }
}

void Game::Initialize()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  SDL_JoystickEventState(SDL_ENABLE);
  // test code - lots of stuff hardcoded
  m_renderer.CreateWindow("Title", 800, 600);
  m_renderer.LoadSprites("data/sprites");
  LoggerManager::SetGlobalLogLevel(LogLevel::ALL);
  LoggerManager::SetLogFolder("logs");

  FontManager::Instance().Initialize("data/fonts/FreeMono.otf");


  SDL_Rect location;
  SDL_Color color;

  location.x = 20;
  location.y = 20;
  location.w = 300;
  location.h = 300;

  color.r = 201;
  color.g = 240;
  color.b = 248;
  color.a = 255;
  std::unique_ptr<Window> window(new Window(location, color, &m_renderer));
  window->Activate();
  
  m_inputManager.Initialize();

  location.w = 80;
  location.h = 30;
  std::unique_ptr<Button> button(new Button(location, "text", &m_renderer));

  button->AddHandler([&]{ this->ShutDownGame(); });

  window->AddWindow(std::move(button));

  m_windowManager.AddWindow(std::move(window));
  m_inputManager.RegisterInputHandler([&](Command* cmd) { return m_windowManager.HandleInput(cmd); }, 10);

}



void Game::HandleInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
     if (event.type == SDL_QUIT)
     {
       m_running = false;
       return;
     }
     m_inputManager.HandleInput(event);
  }
}

// for testing only - can be removed!
void Game::ShutDownGame()
{
  m_running = false;
}


