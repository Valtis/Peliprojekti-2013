#include "Game.h"
#include "Level/Level.h"
#include "Utility/LoggerManager.h"
#include "Graphics/FontManager.h"
// for testing purposes until level manager actually contains levels - feel free to remove
/*
#include "Entity/Entity.h"
#include "Component/InputComponent.h"
#include "Component/GraphicsComponent.h"
#include "Component/LocationComponent.h"
#include "Component/VelocityComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/FactionComponent.h"
*/
#include "Graphics/Camera/EntityTrackingCamera.h"
#include "Component/WalkingAiComponent.h"


// for testing purposes - testing windows -  feel free to remove
#include "UI/Window/Window.h"
#include "UI/Window/Button.h"
#include "UI/Window/TextBox.h"
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
    m_renderer.Draw(m_testDebugCamera.get(), m_levelManager.GetCurrentLevel()->GetEntities(), m_windowManager.GetWindows());
  }
}

void Game::Initialize()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  SDL_JoystickEventState(SDL_ENABLE);
  // test code - lots of stuff hard coded
  m_renderer.CreateWindow("Title", 800, 600);
  m_renderer.LoadSprites("data/sprites/");
  LoggerManager::SetGlobalLogLevel(LogLevel::ALL);
  LoggerManager::SetLogFolder("logs");

  FontManager::Instance().Initialize("data/fonts/FreeMono.otf");
  m_inputManager.Initialize();
  m_inputManager.RegisterInputHandler([&](Command* cmd) { return m_windowManager.HandleInput(cmd); }, 10);

  std::unique_ptr<EntityTrackingCamera> camera(new EntityTrackingCamera);

  m_levelManager.Initialize(m_inputManager, camera);

  m_testDebugCamera = std::move(camera);

  TestWindowCreation();
  m_running = true;

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
  std::unique_ptr<TextBox> textBox(new TextBox("DUE TO MISREADING THE DEADLINE THIS IS KINDA BROKEN. Just pretend we have fixed the collision detection, added health and added level end point", location, &m_renderer));
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


  m_levelManager.GetCurrentLevel()->debugVictoryWindow = window.get();
  m_windowManager.AddWindow(std::move(window));

}

