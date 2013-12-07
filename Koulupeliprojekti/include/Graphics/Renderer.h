

#pragma once

#include <memory>
#include <deque>
#include <vector>
#include <SDL.h>
#include <string>
#include <utility>

#include "SpriteManager.h"

enum class MessageHandling;
class Camera;
class Window;
class Entity;
class Sprite;
class GraphicsComponent;
class LocationComponent;
class Message;
class MessageProcessor;
class Hud;

class Renderer
{
public:
  Renderer();
  virtual ~Renderer();

  SDL_Renderer *GetRenderingContext() { return m_renderer; }
  void LoadSprites(std::string datafilePath);
  void SetWindowSize(int width, int height); 
  
  void RegisterMessageHandlers(MessageProcessor *processor);
  void CreateWindow(std::string title, int width, int height);

  void AddEntity(int id);
  typedef const std::vector<std::unique_ptr<Entity>> EntityVector;
  void Draw( Camera *camera, EntityVector &entities, EntityVector &staticEntities, EntityVector &staticCollidables, const std::deque<std::unique_ptr<Window>> &windows, Hud &hud);

protected:
private:

  MessageHandling HandleTiledSpriteCreation(Message *message);
  

  void ClearScreen();
  void DrawEntities(Camera *camera, EntityVector  &entities, EntityVector &staticEntities, EntityVector &staticCollidables);
  void DrawWindows(const std::deque<std::unique_ptr<Window>> &windows);
  void DrawWindow(std::vector<std::pair<SDL_Rect, SDL_Texture *>>);

  struct SpriteData
  {
    SpriteData() : location(), sprite(nullptr), flip(false)
    {

    }

    SDL_Point location;
    Sprite *sprite;
    bool flip;
  };
    
  std::vector<SpriteData> GetDataForDrawing(SDL_Point topleft, EntityVector  &entities, EntityVector  &staticEntities, EntityVector &staticCollidables);


  void GetEntityData( const std::vector<std::unique_ptr<Entity>> & entities, SDL_Point topleft, std::vector<SpriteData> &retval );

  void DebugDrawCollisionBoxes(EntityVector  &entities, EntityVector &staticCollidables,  SDL_Point topleft);

  void DrawSingleCollisionBox(const std::unique_ptr<Entity> &entity, SDL_Point &topleft );

  bool CullEntity(SDL_Point topleft, Sprite *sprite, LocationComponent *location);
  void SortEntitiesByDrawPriority(std::vector<SpriteData> &drawData);
  void PerformEntityDraw(std::vector<SpriteData> drawdata, SDL_Point point);
  void DrawHud(Hud &hud);

  std::pair<int, int> m_windowSize; // width/height

  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  SpriteManager m_spriteManager;
  MessageProcessor *m_parentProcessor;
};

