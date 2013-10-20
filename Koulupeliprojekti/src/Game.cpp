#include "Game.h"
#include "Level/Level.h"
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
  }
}

void Game::Initialize()
{
  // initialization code here
}

void Game::UpdateGameState()
{
  if (m_gameTick.TickHasPassed())
  {
    m_levelManager.Update(m_gameTick.TicksPassed());
  }
}

void Game::Draw()
{
  if (m_drawTick.TickHasPassed())
  {
    m_renderer.Draw(nullptr, m_levelManager.GetCurrentLevel()->GetEntities(), m_windowManager.GetWindows());
  }
}
