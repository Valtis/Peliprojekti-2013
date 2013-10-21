#pragma once
#include "UI/Window/WindowBase.h"
#include <SDL.h>

class Renderer;
class Window : public WindowBase
{
public:
  Window();
  Window(SDL_Rect location, SDL_Color color, Renderer *renderer);
  ~Window();
private:
  std::string title;
};