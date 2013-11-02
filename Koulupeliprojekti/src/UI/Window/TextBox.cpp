#include "UI/Window/TextBox.h"
#include "UI/Window/TextField.h"
#include "Graphics/TextureFactory.h"
TextBox::TextBox(SDL_Rect location, Renderer *renderer)
{
  SetLocation(location);
  SDL_Color color = { 160, 160, 160, 255};
  SetTexture(TextureFactory::CreateWindowTexture(location.w, location.h, color, renderer));

  std::string text = "This is a test string for testing purposes. This string\ncontains control characters\nso\tthat I see those working. There is also lots of text so I can see the text element works in general!";
  
  std::unique_ptr<TextField> element(new TextField(text, 13, location, renderer));
  AddWindow(std::move(element));
}

TextBox::~TextBox()
{

}