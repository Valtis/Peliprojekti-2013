#include "Game.h"


Game::Game()
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

}

void Game::Draw()
{

}
