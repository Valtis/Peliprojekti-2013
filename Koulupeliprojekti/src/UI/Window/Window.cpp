#include "UI/Window/Window.h"
#include "Graphics/TextureFactory.h"

// test code - move sprite construction to proper class/method
Window::Window(SDL_Rect location, SDL_Color color, Renderer *renderer) : m_dragging(DragStatus::Not_Dragging)
{
  SetRect(location);
  SetTexture(TextureFactory::CreateWindowTexture(location.w, location.h, color, renderer));
}

Window::~Window()
{

}

void Window::OnDrag(int cursorX, int cursorY, int dx, int dy)
{
  if (m_dragging == DragStatus::Not_Dragging)
  {
    if (CoordinateOnAnyWindow(cursorX, cursorY))
    {
      ChildrenCaller([=](ChildPtr &c) { c->OnDrag(cursorX, cursorY, dx, dy); });
      m_dragging = DragStatus::Failed_Drag;
    }
    else 
    {
      m_dragging = DragStatus::Dragging;      
    }
  }

  if (m_dragging == DragStatus::Dragging)
  {
      ModifyX(dx);
      ModifyY(dy);
  }
}

void Window::OnMouseButtonUp(Uint8 button, int cursorX, int cursorY)
{
  if (button == SDL_BUTTON_LEFT)
  {
    m_dragging = DragStatus::Not_Dragging;
  }
  WindowBase::OnMouseButtonUp(button, cursorX, cursorY);
}

void Window::OnMouseButtonDown(Uint8 button, int cursorX, int cursorY)
{
  if (button == SDL_BUTTON_LEFT)
  {
    m_dragging = DragStatus::Not_Dragging;
  }

  WindowBase::OnMouseButtonDown(button, cursorX, cursorY);
}

void Window::OnLosingFocus(int cursorX, int cursorY)
{
  m_dragging = DragStatus::Not_Dragging;
  WindowBase::OnLosingFocus(cursorX, cursorY);
}