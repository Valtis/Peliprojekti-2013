#pragma once
#include "UI/Window/WindowBase.h"
class Renderer;
class TextBox : public WindowBase
{
public:
  TextBox(SDL_Rect location, Renderer *renderer);

  ~TextBox();
private:

};