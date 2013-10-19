#include "UI/Window/WindowManager.h"
#include "UI/Window/Window.h"
#include "Utility/LoggerManager.h"
WindowManager::WindowManager() : m_leftButtonDown(false)
{

}


void WindowManager::HandleEvent(const SDL_Event &event)
{
  switch (event.type)
  {
  case SDL_MOUSEMOTION:
    break;
  case SDL_MOUSEBUTTONDOWN:
    HandleDownButton(event);
    break;
  case SDL_MOUSEBUTTONUP:
    break;
  case SDL_MOUSEWHEEL:
    break;

  default:
    LoggerManager::GetLog(WINDOW_MANAGER_LOG).AddLine(LogLevel::WARNING, "Non-mouse event in WindowManager - shouldn't happen (event type: " + std::to_string(event.type) + ")");
  }
}

void WindowManager::HandleDownButton(const SDL_Event &event)
{
  if (event.button.button == SDL_BUTTON_LEFT)
  {
    m_leftButtonDown = true;
  }

  NotifyWindowOnEvent([=](Window *window) { window->OnMouseButtonDown(event.button.button, event.button.x, event.button.y ); }, event.button.x, event.button.y);
}

void WindowManager::NotifyWindowOnEvent(std::function<void(Window *)> f, int x, int y)
{
  Window * window = GetWindowUnderCoordinates(x, y);
  if (window == nullptr)
  {
    return;
  }

  f(window);
}


Window *WindowManager::GetWindowUnderCoordinates(int x, int y)
{
  UpdateActiveWindow(x, y);
  if (m_windows.size() > 0 && 
    m_windows.front()->GetX() < x && m_windows.front()->GetX() + m_windows.front()->GetWidth() >= x &&
    m_windows.front()->GetY() < y && m_windows.front()->GetY() + m_windows.front()->GetHeight() >= y
    )
  {
    return m_windows[0].get();
  }
  return nullptr;
}

void WindowManager::UpdateActiveWindow(int x, int y)
{
  for (auto iter= m_windows.begin();  iter != m_windows.end(); ++iter)
  {
    if ((*iter)->GetX() < x && (*iter)->GetX() + (*iter)->GetWidth() >= x &&
      (*iter)->GetY() < y && (*iter)->GetY() + (*iter)->GetHeight() >= y)
    {
      std::unique_ptr<Window> temp = std::move(*iter);
      m_windows.erase(iter);
      m_windows.push_front(std::move(temp));
      return;
    }
  }
}



void WindowManager::Draw()
{
  for (auto iter = m_windows.rbegin(); iter != m_windows.rend(); ++iter)
  {
    // draw
  }
}