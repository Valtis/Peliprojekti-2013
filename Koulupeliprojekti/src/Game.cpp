#include "Game.h"
#include "Level/Level.h"
#include "Utility/LoggerManager.h"
// for testing purposes until level manager actually contains levels - feel free to remove
#include "Entity/Entity.h"
// for testing purposes - testing windows -  feel free to remove
#include "UI/Window/Window.h"
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



  while (true)
  {
    m_inputManager.HandleInput();
    UpdateGameState();
    Draw();


    SDL_Event event;

    while (SDL_PollEvent(&event))
    {


      if (event.type == SDL_MOUSEMOTION || event.type ==  SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
      {
        MouseCommand cmd(event);
        m_windowManager.HandleInput(&cmd);
      }
    } 
  }
}

void Game::Initialize()
{
  SDL_Init(SDL_INIT_VIDEO);
  // test code - lots of stuff hardcoded
  m_renderer.CreateWindow("Title", 800, 600);
  m_renderer.LoadSprites("data/sprites");
  LoggerManager::SetGlobalLogLevel(LogLevel::ALL);
  LoggerManager::SetLogFolder("logs");


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

  m_windowManager.AddWindow(std::move(window));

}

void Game::UpdateGameState()
{
  if (m_gameTick.TickHasPassed())
  {
    m_inputManager.HandleInput();
    m_levelManager.Update(m_gameTick.TicksPassed());
  }
}



void Game::Draw()
{
  if (m_drawTick.TickHasPassed())
  {
    // m_renderer.Draw(nullptr, m_levelManager.GetCurrentLevel()->GetEntities(), m_windowManager.GetWindows());
    std::vector<std::unique_ptr<Entity>> e;
    m_renderer.Draw(nullptr, e, m_windowManager.GetWindows());
  }
}
