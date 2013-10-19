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

  virtual void OnLeftMouseButtonDown(int x, int y);
  virtual void OnLeftMouseButtonUp(int x, int y);

  virtual void OnMiddleMouseButtonDown(int x, int y);
  virtual void OnMiddleMouseButtonUp(int x, int y);
  
  virtual void OnRightMouseButtonDown(int x, int y);
  virtual void OnRightMouseButtonUp(int x, int y);

  virtual void OnMouseWheelScrollUp();
  virtual void OnMouseWheelScrollDown();

  virtual void OnDrag(int dx, int dy);
  
  virtual void OnGainingFocus();
  virtual void OnLosingFocus();

  void AddWindow(std::unique_ptr<WindowBase> window);

protected:
  typedef std::unique_ptr<WindowBase> ChildPtr;
  WindowBase();
  void ChildrenCaller(std::function<void(ChildPtr &base)> f);
private:
  
  // add sprite 
  SDL_Rect m_location;
  WindowBase *m_parent;
  std::vector<ChildPtr> m_children;
};