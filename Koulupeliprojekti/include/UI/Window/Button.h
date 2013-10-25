#pragma once
#include "WindowBase.h"
#include <functional>
#include <string>

class Renderer;

class Button : public WindowBase
{
public:
  Button(SDL_Rect location, std::string text, Renderer *renderer);
  ~Button();
  void AddHandler(std::function<void()> handler) { m_handlers.push_back(handler); }
  void OnMouseButtonUp(Uint8 button, int x, int y) override;

//  void SetButtonText(std::string text) { m_text = text; }
//  std::string GetButtonText() { return m_text; }

private:
  std::vector<std::function<void()>> m_handlers;
};