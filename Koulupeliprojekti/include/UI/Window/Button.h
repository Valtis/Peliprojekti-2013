#pragma once
#include "WindowBase.h"
class Button : public WindowBase
{
public:
  Button();
  ~Button();

  void OnLeftMouseButtonUp(int x, int y) override;

private:
  
};