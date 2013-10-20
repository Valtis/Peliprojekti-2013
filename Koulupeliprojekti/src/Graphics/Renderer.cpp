#include "Graphics/Renderer.h"
#include "Graphics/SpriteManager.h"
#include "Graphics/Sprite.h"

#include "Entity/Entity.h"

#include <stdexcept>
#include <string>
#include <algorithm>

Renderer *Renderer::mInstance = nullptr;


Renderer::Renderer()
{

}

Renderer::~Renderer()
{
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
}


Renderer &Renderer::Instance()
{
  if (mInstance == nullptr)
  {
    mInstance = new Renderer();
  }

  return *mInstance;
}

void Renderer::Release()
{
  delete mInstance;
  mInstance = nullptr;
}

void Renderer::CreateWindow(std::string title, int width, int height)
{
  if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &mWindow, &mRenderer) != 0)
  {
    throw std::runtime_error("SDL_CreateWindowAndRenderer failed");
  }

  if (mWindow == nullptr || mRenderer == nullptr)
  {
    throw std::runtime_error("Failed to create window and render context");
  }

  SDL_SetWindowTitle(mWindow, title.c_str());
}

void Renderer::LoadSprites(std::string datafilePath)
{
  m_spriteManager.Initialize(datafilePath);
}

void Renderer::Draw(Camera *camera, const std::vector<std::unique_ptr<Entity>> &entities)
{
  SDL_assert(camera != nullptr);
  ClearScreen();
  DrawEntities(camera, entities);
  SDL_RenderPresent(mRenderer);
}


void Renderer::ClearScreen()
{
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
  SDL_RenderClear(mRenderer);
}

void Renderer::DrawEntities(Camera *camera, const std::vector<std::unique_ptr<Entity>> &entities)
{
  // draw stuff
}
