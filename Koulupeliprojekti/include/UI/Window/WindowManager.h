#pragma once
#include <memory>
#include <deque>
#include <SDL.h>
#include <functional>
class Window;
class WindowManager
{
public:
  WindowManager();
  void Draw();
private:
  void HandleEvent(const SDL_Event &event);
  void HandleDownButton(const SDL_Event &event);
  void HandleUpButton(const SDL_Event &event);
  void HandleMouseMotion(const SDL_Event &event);
  void HandleMouseWheel(const SDL_Event &event);
  void NotifyWindowOnEvent(std::function<void(Window *)> f, int x, int y);
  Window *GetWindowUnderCoordinates(int cursorX, int cursorY);
  void UpdateActiveWindow(int cursorX, int cursorY);
  std::deque<std::unique_ptr<Window>> m_windows;
  bool m_leftButtonDown;
};