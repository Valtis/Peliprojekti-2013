#include "UI/Window/Button.h"

Button::Button()
{
  // initialize graphics 
}

Button::~Button()
{

}

void Button::OnLeftMouseButtonUp(int x, int y)
{
  for (auto handler : m_handlers)
  {
    handler->OnButtonClick();
  }
}