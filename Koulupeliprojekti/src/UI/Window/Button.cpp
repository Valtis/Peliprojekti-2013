#include "UI/Window/Button.h"
#include "Graphics/TextureFactory.h"
Button::Button(SDL_Rect location, std::string text, Renderer *renderer) : m_text(text)
{
  SetRect(location);
  SetTexture(TextureFactory::CreateButton(location.w, location.h, renderer));
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
    handler->OnButtonClick();
  }
}