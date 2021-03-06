#include "UI/Window/WindowManager.h"
#include "UI/Window/Window.h"
#include "Message/Commands/MouseCommand.h"
#include "Utility/LoggerManager.h"
WindowManager::WindowManager() : m_leftButtonDown(false), m_dragStatus(DragStatus::Not_Dragging)
{

}

WindowManager::~WindowManager()
{

}


MessageHandling WindowManager::HandleInput(Command *command)
{
  // todo - catch keyboard events for menu etc
  if (command == nullptr || command->GetType() != MessageType::MOUSE_COMMAND)
  {
    return MessageHandling::PASS_FORWARD;;
  }

  MouseCommand *mouseCommand = static_cast<MouseCommand *>(command);
  return HandleEvent(mouseCommand->GetEvent());

}

MessageHandling WindowManager::HandleEvent( const SDL_Event &event )
{
  std::stringstream ss;
  ss << event.type;
  switch (event.type)
  {
  case SDL_MOUSEMOTION:
    return HandleMouseMotion(event);
    break;
  case SDL_MOUSEBUTTONDOWN:
    return HandleDownButton(event);
    break;
  case SDL_MOUSEBUTTONUP:
    return HandleUpButton(event);
    break;
  case SDL_MOUSEWHEEL:
    return HandleMouseWheel(event);
    break;

  default:
    LoggerManager::GetLog(WINDOW_MANAGER_LOG).AddLine(LogLevel::WARNING, "Non-mouse event in WindowManager - shouldn't happen (event type: " +
      ss.str() + ")");
  }
  return MessageHandling::PASS_FORWARD;
}

MessageHandling WindowManager::HandleMouseMotion( const SDL_Event &event )
{
  if (!m_leftButtonDown || m_windows.empty())
  {
    return MessageHandling::PASS_FORWARD;;
  }

  if (m_dragStatus == DragStatus::Not_Dragging)
  {
    if (GetWindowUnderCoordinates(event.motion.x, event.motion.y) != nullptr)
    {
      m_dragStatus = DragStatus::Dragging;
    }
    else
    {
      m_dragStatus = DragStatus::Failed_Drag;
    }
  }


  if (m_dragStatus == DragStatus::Dragging)
  {
    m_windows[0]->OnDrag(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
    return  MessageHandling::STOP_HANDLING;
  }

  return  MessageHandling::PASS_FORWARD;;
}

MessageHandling WindowManager::HandleDownButton( const SDL_Event &event )
{
  if (event.button.button == SDL_BUTTON_LEFT)
  {
    m_leftButtonDown = true;
  }

  return NotifyWindowUnderCursorOnEvent([=](Window *window) { window->OnMouseButtonDown(event.button.button, event.button.x, event.button.y ); }, event.button.x, event.button.y);
}

MessageHandling WindowManager::HandleUpButton( const SDL_Event &event )
{
  if (event.button.button == SDL_BUTTON_LEFT)
  {
    m_leftButtonDown = false;
    m_dragStatus = DragStatus::Not_Dragging;
  }

  return NotifyWindowUnderCursorOnEvent([=](Window *window) { window->OnMouseButtonUp(event.button.button, event.button.x, event.button.y ); }, event.button.x, event.button.y);
}

MessageHandling WindowManager::HandleMouseWheel( const SDL_Event &event )
{
  int x, y;
  SDL_GetMouseState(&x, &y);
  if (event.wheel.y > 0)
  {
    return NotifyWindowUnderCursorOnEvent([=](Window *window) { window->OnMouseWheelScrollUp(x, y ); }, x, y);
  }
  else if (event.wheel.y < 0)
  {
    return NotifyWindowUnderCursorOnEvent([=](Window *window) { window->OnMouseWheelScrollDown(x, y ); }, x, y);
  }
  return MessageHandling::PASS_FORWARD;
}

MessageHandling WindowManager::NotifyWindowUnderCursorOnEvent( std::function<void(Window *)> f, int x, int y )
{
  Window * window = GetWindowUnderCoordinates(x, y);
  if (window == nullptr)
  {
     return MessageHandling::PASS_FORWARD;;
  }

  f(window);
  return MessageHandling::STOP_HANDLING;;
}


Window *WindowManager::GetWindowUnderCoordinates(int x, int y)
{
  UpdateActiveWindow(x, y);
  if (m_windows.size() > 0 && m_windows[0]->IsActive() &&
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
    if (!(*iter)->IsActive())
    {
      continue;
    }
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

