#pragma once
#include "Window.h"
#include <deque>
class WindowManager
{
public:
private:
  std::deque<Window> m_windows;
};