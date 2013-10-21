#include <Graphics/TextureFactory.h>
#include "Graphics/SurfaceOperations.h"
#include "Graphics/Renderer.h"
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RMASK  0xff000000
#define GMASK  0x00ff0000
#define BMASK  0x0000ff00
#define AMASK  0x000000ff
#else
#define RMASK  0x000000ff
#define GMASK  0x0000ff00
#define BMASK  0x00ff0000
#define AMASK  0xff000000
#endif

void DrawBox(SDL_Rect rect, SDL_Color color, SDL_Surface *surface)
{
  for (int i = 0; i < rect.w; ++i)
  {
    SetColor(i, 0, color, surface);
    SetColor(i, rect.h - 1, color, surface);
  }

  for (int i = 0; i < rect.h; ++i)
  {
    SetColor(0, i, color, surface);
    SetColor(rect.w - 1, i, color, surface);
  }
}

void ColorArea(SDL_Rect rect, SDL_Color color, SDL_Surface *surface)
{
  for (int y = rect.y; y < rect.h; ++y)
  {
    for (int x = rect.x; x < rect.w; ++x) 
    {
      SetColor(x, y, color, surface);
    }
  }
}

SDL_Surface *CreateSurface(int width, int height)
{
  auto surface =  SDL_CreateRGBSurface(0, width, height, 32, RMASK, GMASK, BMASK, AMASK);
  SDL_assert_release(surface != nullptr);
  return surface;
}

SDL_Texture *CreateTexture( Renderer * renderer, SDL_Surface * surface )
{
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer->GetRenderingContext(), surface);
  SDL_assert_release(texture != nullptr);
  SDL_FreeSurface(surface);
  return texture;
}

SDL_Texture *TextureFactory::CreateWindowTexture(int width, int height, SDL_Color color, Renderer *renderer)
{
  SDL_Surface *surface = CreateSurface(width, height);
  SDL_Rect rect = { 0, 0, width, height};

  ColorArea(rect, color, surface);

  SDL_Color red = { 255, 0, 0, 255 };
  DrawBox(rect, red, surface);

  return CreateTexture(renderer, surface);
}

SDL_Texture *TextureFactory::CreateButton(int width, int height, Renderer *renderer)
{
  SDL_Surface *surface = CreateSurface(width, height);
  SDL_Rect rect = { 0, 0, width, height };
  SDL_Color color = { 195, 195, 195, 255};
  
  ColorArea(rect, color, surface);
  
  
  return CreateTexture(renderer, surface);
}


