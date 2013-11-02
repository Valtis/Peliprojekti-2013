#include "UI/Window/TextBox.h"
#include "UI/Window/TextField.h"
#include "Graphics/TextureFactory.h"
TextBox::TextBox(std::string text, SDL_Rect location, Renderer *renderer)
{
  SetLocation(location);
  SDL_Color color = { 160, 160, 160, 255};
  SetTexture(TextureFactory::CreateWindowTexture(location.w, location.h, color, renderer));

  std::unique_ptr<TextField> element(new TextField(text, 13, location, renderer));
  AddWindow(std::move(element));
}

TextBox::~TextBox()
{

}