#pragma once
#include "UI/Window/WindowBase.h"

class Renderer;

class TextField : public WindowBase
{
public:
  TextField(std::string text, int fontSize, SDL_Rect location, Renderer *renderer);
  
private:
  
};