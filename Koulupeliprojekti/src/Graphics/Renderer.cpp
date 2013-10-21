#include "Graphics/Renderer.h"
#include "Graphics/SpriteManager.h"
#include "Graphics/Sprite.h"
#include "UI/Window/Window.h"
#include "Entity/Entity.h"

#include <stdexcept>
#include <string>
#include <algorithm>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
}



void Renderer::CreateWindow(std::string title, int width, int height)
{
  if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &m_window, &m_renderer) != 0)
  {
    throw std::runtime_error("SDL_CreateWindowAndRenderer failed");
  }

  if (m_window == nullptr || m_renderer == nullptr)
  {
    throw std::runtime_error("Failed to create window and render context");
  }

  SDL_SetWindowTitle(m_window, title.c_str());
}

void Renderer::LoadSprites(std::string datafilePath)
{
  m_spriteManager.Initialize(this, datafilePath);
}

void Renderer::Draw(Camera *camera, const std::vector<std::unique_ptr<Entity>> &entities, const std::deque<std::unique_ptr<Window>> &windows)
{
 // SDL_assert(camera != nullptr);
  ClearScreen();
  DrawEntities(camera, entities);
  DrawWindows(windows);
  SDL_RenderPresent(m_renderer);
}


void Renderer::ClearScreen()
{
  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
  SDL_RenderClear(m_renderer);
}

void Renderer::DrawEntities(Camera *camera, const std::vector<std::unique_ptr<Entity>> &entities)
{
  // draw stuff
}

void Renderer::DrawWindows(const std::deque<std::unique_ptr<Window>> &windows)
{
  for (auto iter = windows.rbegin(); iter != windows.rend(); ++iter)
  {
    if (!(*iter)->IsActive())
    {
      continue;
    }
    DrawWindow((*iter)->GetTextures());
  }
}

void Renderer::DrawWindow(std::vector<std::pair<SDL_Rect, SDL_Texture *>> windowTextures)
{
  for (auto texture : windowTextures)
  {
    SDL_RenderCopy(m_renderer, texture.second, nullptr, &(texture.first));
  }
}