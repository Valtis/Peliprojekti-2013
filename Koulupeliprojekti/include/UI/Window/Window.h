#pragma once
#include "UI/Window/WindowBase.h"

class Window : public WindowBase
{
public:
  Window();
private:
  std::string title;
};