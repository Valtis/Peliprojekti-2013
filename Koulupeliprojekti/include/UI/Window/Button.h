#pragma once
#include "WindowBase.h"
#include "UI/Window/ButtonHandler.h"
#include <string>

class Renderer;
class Button : public WindowBase
{
public:
  Button(SDL_Rect location, std::string text, Renderer *renderer);
  ~Button();
  void AddHandler(ButtonHandler *handler) { m_handlers.push_back(handler); }
  void OnMouseButtonUp(Uint8 button, int x, int y) override;

  void SetButtonText(std::string text) { m_text = text; }
  std::string GetButtonText() { return m_text; }

private:
  std::vector<ButtonHandler *> m_handlers;
  std::string m_text; 
};