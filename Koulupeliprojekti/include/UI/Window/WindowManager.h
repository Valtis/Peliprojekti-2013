#pragma once
#include <memory>
#include <deque>
#include <SDL.h>
#include <functional>
#include "UI/Window/Window.h"
class Command;

class WindowManager
{
public:
  WindowManager();
  ~WindowManager();
  bool HandleInput(Command *command);

  void AddWindow(std::unique_ptr<Window> window) { m_windows.push_back(std::move(window)); }
  const std::deque<std::unique_ptr<Window>> &GetWindows() { return m_windows; }
private:
  bool HandleEvent(const SDL_Event &event);
  bool HandleDownButton(const SDL_Event &event);
  bool HandleUpButton(const SDL_Event &event);
  bool HandleMouseMotion(const SDL_Event &event);
  bool HandleMouseWheel(const SDL_Event &event);
  bool NotifyWindowUnderCursorOnEvent(std::function<void(Window *)> f, int x, int y);
  Window *GetWindowUnderCoordinates(int cursorX, int cursorY);
  void UpdateActiveWindow(int cursorX, int cursorY);
  std::deque<std::unique_ptr<Window>> m_windows;
  bool m_leftButtonDown;
};