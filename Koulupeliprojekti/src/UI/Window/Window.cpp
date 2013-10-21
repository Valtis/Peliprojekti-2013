#include "UI/Window/Window.h"
#include "Graphics/Renderer.h"
#include "Graphics/SurfaceOperations.h"

Window::Window() 
{
  // create common window features (title bar - close button)
}
// test code - move sprite construction to proper class/method
Window::Window(SDL_Rect location, SDL_Color color, Renderer *renderer)
{
  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  SDL_Surface *surface = SDL_CreateRGBSurface(0, location.w, location.h, 32, rmask, gmask, bmask, amask);
  SDL_assert(surface != nullptr);
  
  for (int y = 0; y < location.h; ++y)
  {
    for (int x = 0; x < location.w; ++x) 
    {
      SetColor(x, y, color, surface);
    }
  }
  
  SDL_Color red = { 255, 0, 0, 255 };
  for (int i = 0; i < location.w; ++i)
  {
    SetColor(i, 0, red, surface);
    SetColor(i, location.h - 1, red, surface);
  }

  for (int i = 0; i < location.h; ++i)
  {
    SetColor(0, i, red, surface);
    SetColor(location.w - 1, i, red, surface);
  }

  SetRect(location);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer->GetRenderingContext(), surface);
  SDL_assert(texture != nullptr);

  SDL_FreeSurface(surface);
  
  SetTexture(texture);
}

Window::~Window()
{

}

