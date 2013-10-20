

#pragma once

#include <memory>
#include <vector>
#include <SDL.h>
#include <string>


#include "SpriteManager.h"

class Camera;
class Entity;
class Sprite;

class Renderer
{
public:

  virtual ~Renderer();

  static Renderer &Instance();
  static void Release();

  SDL_Renderer *GetRenderingContext() { return mRenderer; }
  void LoadSprites(std::string datafilePath);

  void CreateWindow(std::string title, int width, int height);

  void AddEntity(int id);
  void Draw(Camera *camera, const std::vector<std::unique_ptr<Entity>> &entities);

protected:
private:

  static Renderer *mInstance;
  Renderer();

  void ClearScreen();
  void DrawEntities(Camera *camera, const std::vector<std::unique_ptr<Entity>> &entities);
  


  SDL_Window *mWindow;
  SDL_Renderer *mRenderer;
  SpriteManager m_spriteManager;
};

