#pragma once
#include "UI/Window/WindowBase.h"

class Window : public WindowBase
{
public:
  Window();
  ~Window();
private:
  std::string title;
};