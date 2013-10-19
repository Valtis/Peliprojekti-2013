#pragma once
#include "Window.h"
#include <memory>
#include <deque>
class WindowManager
{
public:
  WindowManager();
  void Draw();
private:
  void UpdateActiveWindow(int cursorX, int cursorY);
  Window *GetWindowUnderCoordinates(int cursorX, int cursorY);
  std::deque<std::unique_ptr<Window>> m_windows;
};