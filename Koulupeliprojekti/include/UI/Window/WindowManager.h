#pragma once
#include <memory>
#include <deque>
#include <SDL.h>
#include <functional>
#include "UI/Window/Window.h"
enum class MessageHandling;
class Command;

class WindowManager
{
public:
  WindowManager();
  ~WindowManager();
  MessageHandling HandleInput(Command *command);

  void AddWindow(std::unique_ptr<Window> window) { m_windows.push_back(std::move(window)); }
  const std::deque<std::unique_ptr<Window>> &GetWindows() { return m_windows; }
private:
  MessageHandling HandleEvent(const SDL_Event &event);
  MessageHandling HandleDownButton(const SDL_Event &event);
  MessageHandling HandleUpButton(const SDL_Event &event);
  MessageHandling HandleMouseMotion(const SDL_Event &event);
  MessageHandling HandleMouseWheel(const SDL_Event &event);
  MessageHandling NotifyWindowUnderCursorOnEvent(std::function<void(Window *)> f, int x, int y);
  Window *GetWindowUnderCoordinates(int cursorX, int cursorY);
  void UpdateActiveWindow(int cursorX, int cursorY);
  std::deque<std::unique_ptr<Window>> m_windows;
  bool m_leftButtonDown;
  
  DragStatus m_dragStatus;
};