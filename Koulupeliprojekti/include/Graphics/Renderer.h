

#pragma once

#include <memory>
#include <deque>
#include <vector>
#include <SDL.h>
#include <string>
#include <utility>

#include "SpriteManager.h"

class Camera;
class Window;
class Entity;
class Sprite;
class GraphicsComponent;
class LocationComponent;


class Renderer
{
public:
  Renderer();
  virtual ~Renderer();

  SDL_Renderer *GetRenderingContext() { return m_renderer; }
  void LoadSprites(std::string datafilePath);
  

  void CreateWindow(std::string title, int width, int height);

  void AddEntity(int id);
  typedef const std::vector<std::unique_ptr<Entity>> EntityVector;
  void Draw(
    Camera *camera, 
    EntityVector &entities, 
    EntityVector &staticEntities, 
    EntityVector &staticCollidables,
    const std::deque<std::unique_ptr<Window>> &windows);

protected:
private:
  

  void ClearScreen();
  void DrawEntities(Camera *camera, EntityVector  &entities, EntityVector &staticEntities, EntityVector &staticCollidables);
  void DrawWindows(const std::deque<std::unique_ptr<Window>> &windows);
  void DrawWindow(std::vector<std::pair<SDL_Rect, SDL_Texture *>>);

  std::vector<std::pair<SDL_Point, Sprite *>> GetDataForDrawing(SDL_Point topleft, EntityVector  &entities, EntityVector  &staticEntities, EntityVector &staticCollidables);

  void GetEntityData( const std::vector<std::unique_ptr<Entity>> & entities, SDL_Point topleft, std::vector<std::pair<SDL_Point, Sprite *>> &retval );

  void DebugDrawCollisionBoxes(const std::vector<std::unique_ptr<Entity>> &entities, SDL_Point topleft);

  bool CullEntity(SDL_Point topleft, Sprite *sprite, LocationComponent *location);
  void SortEntitiesByDrawPriority(std::vector<std::pair<SDL_Point, Sprite *>> &drawData);
  void PerformEntityDraw(std::vector<std::pair<SDL_Point, Sprite *>> drawdata, SDL_Point point);


  std::pair<int, int> m_windowSize; // width/height

  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  SpriteManager m_spriteManager;
};

