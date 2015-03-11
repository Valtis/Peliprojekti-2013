#pragma once

#include <vector>
#include <memory>
#include <SDL.h>  
#include <functional>

// move the enum to proper file, here for now (must be usable from windowmanager.h in the future)

class WindowBase
{
public:
  
  virtual ~WindowBase();

  bool IsActive() { return m_isActive; }
  void Activate() { m_isActive = true; }
  void Deactivate() { m_isActive = false; }

  int GetX() { return m_location.x; }
  int GetY() { return m_location.y; }
  void SetX(int x) { m_location.x = x; }
  void SetY(int y) { m_location.y = y; }
  void ModifyX(int dx) { m_location.x += dx; }
  void ModifyY(int dy) { m_location.y += dy; }
  
  int GetRelativeX();
  int GetRelativeY();

  int GetWidth() { return m_location.w; }
  int GetHeight() { return m_location.h; }

  SDL_Rect GetRect() { return m_location; }
  void SetLocation(SDL_Rect rect) { m_location = rect; }

  void SetParent(WindowBase *parent) { m_parent = parent; }

  virtual void OnMouseButtonDown(Uint8 button, int cursorX, int cursorY);
  virtual void OnMouseButtonUp(Uint8 button, int cursorX, int cursorY);

  virtual void OnMouseWheelScrollUp(int cursorX, int cursorY);
  virtual void OnMouseWheelScrollDown(int cursorX, int cursorY);

  virtual void OnDrag(int cursorX, int cursorY, int dx, int dy);
  
  virtual void OnGainingFocus(int cursorX, int cursorY);
  virtual void OnLosingFocus(int cursorX, int cursorY);

  void AddWindow(std::unique_ptr<WindowBase> window);

  std::vector<std::pair<SDL_Rect, SDL_Texture *>> GetTextures();

protected:
  typedef std::unique_ptr<WindowBase> ChildPtr;
  WindowBase();
  void ChildrenCaller(std::function<void(ChildPtr &base)> f);
  bool CoordinateOnWindow(ChildPtr &c, int x, int y);
  bool CoordinateOnAnyWindow(int x, int y);

  void SetTexture(SDL_Texture *texture);
private:
  
  SDL_Texture *m_texture;
  SDL_Rect m_location;
  WindowBase *m_parent;
  std::vector<ChildPtr> m_children;
  bool m_isActive;
};
