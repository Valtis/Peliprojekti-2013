#include "UI/Window/WindowBase.h"
#include "Utility/LoggerManager.h"
WindowBase::WindowBase() : m_parent(nullptr), m_isActive(false), m_texture(nullptr)
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

void WindowBase::OnMouseButtonDown(Uint8 button, int cursorX, int cursorY)
{
  ChildrenCaller([=](ChildPtr &c) { if (CoordinateOnWindow(c, cursorX, cursorY)) c->OnMouseButtonDown(button, cursorX, cursorY); });
}

void WindowBase::OnMouseButtonUp(Uint8 button, int cursorX, int cursorY)
{
  ChildrenCaller([=](ChildPtr &c) { if (CoordinateOnWindow(c, cursorX, cursorY)) c->OnMouseButtonUp(button, cursorX, cursorY); });
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
  std::string line = "CursorX: " + std::to_string(cursorX) + "\tCursorY: " + std::to_string(cursorY) + "\tdx: " + std::to_string(dx) + "\tdy: " + std::to_string(dy);
  LoggerManager::GetLog("debug.log").AddLine(LogLevel::DEBUG, line);
  // owned windows should hold still - only main window should change its location
  
  LoggerManager::GetLog("debug.log").AddLine(LogLevel::DEBUG, "OldX: " + std::to_string(m_location.x) + "\tOldY: " + std::to_string(m_location.y));
  if (m_parent == nullptr)
  {
    m_location.x += dx;
    m_location.y += dy;
  }


  LoggerManager::GetLog("debug.log").AddLine(LogLevel::DEBUG, "NewX: " + std::to_string(m_location.x) + "\tNewY: " + std::to_string(m_location.y));
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

std::vector<std::pair<SDL_Rect, SDL_Texture *>> WindowBase::GetTextures()
{
  std::vector<std::pair<SDL_Rect, SDL_Texture *>> textures;

  SDL_Rect rect;
  rect.x = GetRelativeX();
  rect.y = GetRelativeY();
  rect.w = m_location.w;
  rect.h = m_location.h;

  textures.push_back(std::make_pair(rect, m_texture));
  for (auto &child : m_children)
  {
    auto childTextures = child->GetTextures();
    for (auto texture : childTextures)
    {
      textures.push_back(texture);
    }
  }

  return textures;
}


void WindowBase::SetTexture(SDL_Texture *texture)
{
  if (m_texture != nullptr)
  {
    SDL_DestroyTexture(texture);
  }
  m_texture = texture;
}