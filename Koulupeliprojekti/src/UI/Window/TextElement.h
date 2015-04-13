#pragma once
#include "UI/Window/WindowBase.h"

enum class TextAlingment { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };
class Renderer; 
class TextElement : public WindowBase
{
public:
  TextElement(std::string text, int fontSize, TextAlingment alignment, SDL_Rect location, Renderer *renderer);
private:
};