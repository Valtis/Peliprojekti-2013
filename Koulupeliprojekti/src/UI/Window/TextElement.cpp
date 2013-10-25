#include "UI/Window/TextElement.h"
#include "Graphics/FontManager.h"
TextElement::TextElement(std::string text, int fontSize, TextAlingment alignment, SDL_Rect location, Renderer *renderer)
{
  double width = FontManager::Instance().GetFontWidth(fontSize);
  double height = FontManager::Instance().GetFontHeight(fontSize);

  location.y = (location.h/2 - height/2); // align text so that it's halfway height wise

  switch (alignment)
  {
  case TextAlingment::ALIGN_LEFT:
    location.x = 5;
    break;
  case TextAlingment::ALIGN_CENTER:
    location.x = (location.w/2 - width/2);
    break;
  case TextAlingment::ALIGN_RIGHT:
    location.x = location.w - width - 5;
    break;
  }

  SetTexture(FontManager::Instance().GetTextSingleLine(renderer, text, fontSize));

  SetRect(location);
}