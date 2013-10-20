

#pragma once

#include <memory>
#include <deque>
#include <vector>
#include <SDL.h>
#include <string>


#include "SpriteManager.h"

class Camera;
class Window;
class Entity;
class Sprite;

class Renderer
{
public:
  Renderer();
  virtual ~Renderer();

  SDL_Renderer *GetRenderingContext() { return m_renderer; }
  void LoadSprites(std::string datafilePath);
  

  void CreateWindow(std::string title, int width, int height);

  void AddEntity(int id);
  void Draw(Camera *camera, const std::vector<std::unique_ptr<Entity>> &entities, const std::deque<std::unique_ptr<Window>> &windows);

protected:
private:
  

  void ClearScreen();
  void DrawEntities(Camera *camera, const std::vector<std::unique_ptr<Entity>> &entities);
  void DrawWindows(const std::deque<std::unique_ptr<Window>> &windows);
  void DrawWindow(std::vector<std::pair<SDL_Rect, SDL_Texture *>>);


  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  SpriteManager m_spriteManager;
};

