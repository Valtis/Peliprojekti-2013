#include "UI/Window/Button.h"
#include "UI/Window/TextElement.h"
#include "Graphics/TextureFactory.h"
Button::Button(SDL_Rect location, std::string text, Renderer *renderer)
{
  SetLocation(location);
  SetTexture(TextureFactory::CreateButton(location.w, location.h, renderer));

  std::unique_ptr<TextElement> element(new TextElement(text, 13, TextAlingment::ALIGN_CENTER, location, renderer));
  AddWindow(std::move(element));
}

Button::~Button()
{

}

void Button::OnMouseButtonUp(Uint8 button, int x, int y)
{
  if (button != SDL_BUTTON_LEFT)
  {
    return;
  }

  for (auto handler : m_handlers)
  {
    handler();
  }
}