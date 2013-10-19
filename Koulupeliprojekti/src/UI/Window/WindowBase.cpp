#include "UI/Window/WindowBase.h"

WindowBase::WindowBase() : m_parent(nullptr)
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

void WindowBase::OnLeftMouseButtonDown(int x, int y)
{
  ChildrenCaller([&](ChildPtr &c) { c->OnLeftMouseButtonDown(x, y); });
}

void WindowBase::OnLeftMouseButtonUp(int x, int y)
{
  ChildrenCaller([&](ChildPtr &c) { c->OnLeftMouseButtonUp(x, y); });
}

void WindowBase::OnMiddleMouseButtonDown(int x, int y)
{
  ChildrenCaller([&](ChildPtr &c) { c->OnMiddleMouseButtonDown(x, y); });
}
void WindowBase::OnMiddleMouseButtonUp(int x, int y)
{
  ChildrenCaller([&](ChildPtr &c) { c->OnMiddleMouseButtonUp(x, y); });
}

void WindowBase::OnRightMouseButtonDown(int x, int y)
{
  ChildrenCaller([&](ChildPtr &c) { c->OnRightMouseButtonDown(x, y); });
}
void WindowBase::OnRightMouseButtonUp(int x, int y)
{
  ChildrenCaller([&](ChildPtr &c) { c->OnRightMouseButtonUp(x, y); });
}

void WindowBase::OnMouseWheelScrollUp()
{
  ChildrenCaller([&](ChildPtr &c) { c->OnMouseWheelScrollUp(); });
}
void WindowBase::OnMouseWheelScrollDown()
{
  ChildrenCaller([&](ChildPtr &c) { c->OnMouseWheelScrollDown(); });
}

void WindowBase::OnDrag(int dx, int dy)
{
  // owned windows should hold still - only main window should change its location
  if (m_parent == nullptr)
  {
    m_location.x += dx;
    m_location.y += dy;
  }

  ChildrenCaller([&](ChildPtr &c) { c->OnDrag(dx, dy); });
}

void WindowBase::OnGainingFocus()
{
  ChildrenCaller([&](ChildPtr &c) { c->OnGainingFocus(); });
}

void WindowBase::OnLosingFocus()
{
  ChildrenCaller([&](ChildPtr &c) { c->OnLosingFocus(); });
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