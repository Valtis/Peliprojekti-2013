#pragma once
#include "UI/Window/WindowBase.h"
#include <SDL.h>
#include <string>
enum class DragStatus { Not_Dragging, Failed_Drag, Dragging };

class Renderer;

class Window : public WindowBase
{
public:

  Window(SDL_Rect location, SDL_Color color, Renderer *renderer);
  void OnDrag(int cursorX, int cursorY, int dx, int dy) override;
  void OnMouseButtonUp(Uint8 button, int cursorX, int cursorY) override;
  void OnMouseButtonDown(Uint8 button, int cursorX, int cursorY) override;
  void OnLosingFocus(int cursorX, int cursorY) override;
  ~Window();
private:
  std::string title;
  DragStatus m_dragging;
};
