#include "UI/Window/Button.h"

Button::Button()
{
  // initialize graphics 
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