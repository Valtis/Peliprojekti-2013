#pragma once
#include <SDL.h>
class Renderer;
namespace TextureFactory
{
  SDL_Texture *CreateWindowTexture(int width, int height, SDL_Color color, Renderer *renderer);

}