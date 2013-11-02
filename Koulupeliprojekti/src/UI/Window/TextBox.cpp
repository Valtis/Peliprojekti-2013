#include "UI/Window/TextBox.h"
#include "Graphics/TextureFactory.h"

TextBox::TextBox(SDL_Rect location, Renderer *renderer)
{
  SetLocation(location);
  SDL_Color color = { 160, 160, 160, 255};
  SetTexture(TextureFactory::CreateWindowTexture(location.w, location.h, color, renderer));

 // std::unique_ptr<TextElement> element(new TextElement(text, 13, TextAlingment::ALIGN_CENTER, location, renderer));
  //AddWindow(std::move(element));
}

TextBox::~TextBox()
{

}