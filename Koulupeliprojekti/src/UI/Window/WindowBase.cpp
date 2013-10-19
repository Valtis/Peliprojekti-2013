#include "UI/Window/WindowBase.h"

WindowBase::WindowBase() : m_parent(nullptr), m_isActive(false)
{
  m_location.x = 0;
  m_location.y = 0;
  m_location.w = 0;
  m_location.h = 0;
}

WindowBase::~WindowBase()
{

}

int WindowBase::GetRelativeX()
{
  int x = GetX();
  if (m_parent != nullptr)
  {
    x += m_parent->GetRelativeX();
  }
  return x;
}

int WindowBase::GetRelativeY()
{
  int y = GetY();
  if (m_parent != nullptr)
  {
    y += m_parent->GetRelativeY();
  }
  return y;
}

void WindowBase::OnLeftMouseButtonDown(int cursorX, int cursorY)
{
  ChildrenCaller([=](ChildPtr &c) { if (CoordinateOnWindow(c, cursorX, cursorY)) c->OnLeftMouseButtonDown(cursorX, cursorY); });
}

void WindowBase::OnLeftMouseButtonUp(int cursorX, int cursorY)
{
  ChildrenCaller([=](ChildPtr &c) { if (CoordinateOnWindow(c, cursorX, cursorY)) c->OnLeftMouseButtonUp(cursorX, cursorY); });
}

void WindowBase::OnMiddleMouseButtonDown(int cursorX, int cursorY)
{
  ChildrenCaller([=](ChildPtr &c) { if (CoordinateOnWindow(c, cursorX, cursorY))  c->OnMiddleMouseButtonDown(cursorX, cursorY); });
}
void WindowBase::OnMiddleMouseButtonUp(int cursorX, int cursorY)
{
  ChildrenCaller([=](ChildPtr &c) { if (CoordinateOnWindow(c, cursorX, cursorY)) c->OnMiddleMouseButtonUp(cursorX, cursorY); });
}

void WindowBase::OnRightMouseButtonDown(int cursorX, int cursorY)
{
  ChildrenCaller([=](ChildPtr &c) { if (CoordinateOnWindow(c, cursorX, cursorY)) c->OnRightMouseButtonDown(cursorX, cursorY); });
}
void WindowBase::OnRightMouseButtonUp(int cursorX, int cursorY)
{
  ChildrenCaller([=](ChildPtr &c) { c->OnRightMouseButtonUp(cursorX, cursorY); });
}

void WindowBase::OnMouseWheelScrollUp(int cursorX, int cursorY)
{
  ChildrenCaller([=](ChildPtr &c) { if (CoordinateOnWindow(c, cursorX, cursorY)) c->OnMouseWheelScrollUp(cursorX, cursorY); });
}
void WindowBase::OnMouseWheelScrollDown(int cursorX, int cursorY)
{
  ChildrenCaller([=](ChildPtr &c) { if (CoordinateOnWindow(c, cursorX, cursorY)) c->OnMouseWheelScrollDown(cursorX, cursorY); });
}

void WindowBase::OnDrag(int cursorX, int cursorY, int dx, int dy)
{
  // owned windows should hold still - only main window should change its location
  if (m_parent == nullptr)
  {
    m_location.x += dx;
    m_location.y += dy;
  }

  ChildrenCaller([=](ChildPtr &c) { if (CoordinateOnWindow(c, cursorX, cursorY)) c->OnDrag(cursorX, cursorY, dx, dy); });
}

void WindowBase::OnGainingFocus(int cursorX, int cursorY)
{
  ChildrenCaller([=](ChildPtr &c) { c->OnGainingFocus(cursorX, cursorY); });
}

void WindowBase::OnLosingFocus(int cursorX, int cursorY)
{
  ChildrenCaller([=](ChildPtr &c) { c->OnLosingFocus(cursorX, cursorY); });
}

void WindowBase::AddWindow(std::unique_ptr<WindowBase> window)
{
  window->SetParent(this);
  m_children.push_back(std::move(window));
}

void WindowBase::ChildrenCaller(std::function<void(ChildPtr &base)> f)
{
  for (auto &child : m_children)
  {
    f(child);
  }
}

bool WindowBase::CoordinateOnWindow(ChildPtr &c, int x, int y)
{
  int childX = c->GetRelativeX();
  int childY = c->GetRelativeY();
  int childWidth = c->m_location.w;
  int childHeight = c->m_location.h;

  if (childX < x && childX + childWidth >= x &&
      childY < y && childX + childHeight >= y)
  {
    return true;
  }

  return false;
}