#pragma once
#include <vector>
#include <memory>
#include <SDL.h>
#include <functional>

class WindowBase
{
public:
  
  virtual ~WindowBase();

  int GetX() { return m_location.x; }
  int GetY() { return m_location.y; }
  int GetRelativeX();
  int GetRelativeY();

  int GetWidth() { return m_location.w; }
  int GetHeight() { return m_location.h; }

  void SetParent(WindowBase *parent) { m_parent = parent; }

  virtual void OnLeftMouseButtonDown(int cursorX, int cursorY);
  virtual void OnLeftMouseButtonUp(int cursorX, int cursorY);

  virtual void OnMiddleMouseButtonDown(int cursorX, int cursorY);
  virtual void OnMiddleMouseButtonUp(int cursorX, int cursorY);
  
  virtual void OnRightMouseButtonDown(int cursorX, int cursorY);
  virtual void OnRightMouseButtonUp(int cursorX, int cursorY);

  virtual void OnMouseWheelScrollUp(int cursorX, int cursorY);
  virtual void OnMouseWheelScrollDown(int cursorX, int cursorY);

  virtual void OnDrag(int cursorX, int cursorY, int dx, int dy);
  
  virtual void OnGainingFocus(int cursorX, int cursorY);
  virtual void OnLosingFocus(int cursorX, int cursorY);

  void AddWindow(std::unique_ptr<WindowBase> window);

protected:
  typedef std::unique_ptr<WindowBase> ChildPtr;
  WindowBase();
  void ChildrenCaller(std::function<void(ChildPtr &base)> f);
  bool CoordinateOnWindow(ChildPtr &c, int x, int y);
private:
  
  // add sprite 
  SDL_Rect m_location;
  WindowBase *m_parent;
  std::vector<ChildPtr> m_children;
};