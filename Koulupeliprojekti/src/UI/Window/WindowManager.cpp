#include "UI/Window/WindowManager.h"

WindowManager::WindowManager()
{

}

void WindowManager::UpdateActiveWindow(int x, int y)
{
  for (auto iter= m_windows.begin();  iter != m_windows.end(); ++iter)
  {
    if ((*iter)->GetX() > x && (*iter)->GetX() + (*iter)->GetWidth() <= x &&
        (*iter)->GetY() > y && (*iter)->GetY() + (*iter)->GetHeight() <= y)
    {
      std::unique_ptr<Window> temp = std::move(*iter);
      m_windows.erase(iter);
      m_windows.push_front(std::move(temp));
      return;
    }
  }
}

Window *WindowManager::GetWindowUnderCoordinates(int x, int y)
{
  UpdateActiveWindow(x, y);
  if (m_windows.size() > 0 && 
      m_windows.front()->GetX() > x && m_windows.front()->GetX() + m_windows.front()->GetWidth() <= x &&
      m_windows.front()->GetY() > y && m_windows.front()->GetY() + m_windows.front()->GetHeight() <= y
    )
  {
    return m_windows[0].get();
  }
  return nullptr;
}

/*


*/