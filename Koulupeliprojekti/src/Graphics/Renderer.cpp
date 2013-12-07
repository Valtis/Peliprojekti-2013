#include "Graphics/Renderer.h"
#include "Graphics/SpriteManager.h"
#include "Graphics/Sprite.h"
#include "Graphics/Camera/Camera.h"
#include "Component/GraphicsComponent.h"
#include "Component/LocationComponent.h"
#include "Message/MessageProcessor.h"
#include "Message/NewTiledSpriteMessage.h"
// collision is for debugging - can be removed
#include "Component/CollisionComponent.h"
#include "UI/Window/Window.h"
#include "UI/Hud.h"
#include "Entity/Entity.h"

#include <stdexcept>
#include <string>
#include <algorithm>



Renderer::Renderer() : m_window(nullptr), m_renderer(nullptr), m_parentProcessor(nullptr)
{

}

Renderer::~Renderer()
{
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
}

void Renderer::RegisterMessageHandlers(MessageProcessor *processor)
{
  m_parentProcessor = processor;
  m_parentProcessor->RegisterMessageHandler(MessageType::CREATE_NEW_TILED_SPRITE, Priority::HIGHEST, 
    [&](Message *message) { return this->HandleTiledSpriteCreation(message); }
  );
}


void Renderer::CreateWindow(std::string title, int width, int height)
{
  if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &m_window, &m_renderer) != 0)
  {
    throw std::runtime_error("SDL_CreateWindowAndRenderer failed");
  }

  if (m_window == nullptr || m_renderer == nullptr)
  {
    throw std::runtime_error("Failed to create window and render context");
  }

  SDL_SetWindowTitle(m_window, title.c_str());

  m_windowSize.first = width;
  m_windowSize.second = height;
}
void Renderer::SetWindowSize(int width, int height)
{ 
  m_windowSize.first = width; 
  m_windowSize.second = height;
}


void Renderer::LoadSprites(std::string datafilePath)
{
  m_spriteManager.Initialize(this, datafilePath);
}

void Renderer::Draw( Camera *camera, EntityVector &entities, 
                    EntityVector &staticEntities, 
                    EntityVector &staticCollidables, 
                    const std::deque<std::unique_ptr<Window>> &windows, 
                    Hud &hud )
{
  SDL_assert_release(camera != nullptr);
  ClearScreen();

  DrawEntities(camera, entities, staticEntities, staticCollidables);
  DrawHud(hud);
  DrawWindows(windows);
  SDL_RenderPresent(m_renderer);
}


void Renderer::ClearScreen()
{
  SDL_SetRenderDrawColor(m_renderer, 204, 255, 255, 255);
  SDL_RenderClear(m_renderer);
}

void Renderer::DrawEntities( Camera *camera, EntityVector &entities, EntityVector &staticEntities, EntityVector &staticCollidables )
{
  SDL_Point topleft;
  topleft.x = camera->GetX() - m_windowSize.first/2;
  topleft.y = camera->GetY() - m_windowSize.second/1.3;

  std::vector<SpriteData> spriteIds = GetDataForDrawing(topleft, entities, staticEntities, staticCollidables);
  SortEntitiesByDrawPriority(spriteIds);
  PerformEntityDraw(spriteIds, topleft);

  //DebugDrawCollisionBoxes(entities, staticCollidables, topleft);
}

std::vector<Renderer::SpriteData> Renderer::GetDataForDrawing( SDL_Point topleft, EntityVector &entities, EntityVector &staticEntities, EntityVector &staticCollidables )
{
  std::vector<SpriteData> retval;
  GetEntityData(entities, topleft, retval);
  GetEntityData(staticEntities, topleft, retval);
  GetEntityData(staticCollidables, topleft, retval);
  return retval;
}

void Renderer::GetEntityData( const std::vector<std::unique_ptr<Entity>> & entities, SDL_Point topleft, std::vector<SpriteData> &retval )
{
  for (auto &entity : entities)
  {
    SpriteData data;

    auto location = static_cast<LocationComponent *>(entity->GetComponent(ComponentType::LOCATION));
    auto graphics = static_cast<GraphicsComponent *>(entity->GetComponent(ComponentType::GRAPHICS));
    if (location == nullptr || graphics == nullptr || !graphics->IsVisible())
    {
      continue;
    }
    
    data.sprite = m_spriteManager.GetSprite(graphics->GetSpriteID());
    SDL_assert_release(data.sprite != nullptr);

    if (CullEntity(topleft, data.sprite, location))
    {
      continue;
    }

    data.flip = location->GetDirection() != Direction::LEFT;
    SDL_Point loc = { (int)location->GetX(), (int)location->GetY() };
    data.location = loc;
    retval.push_back(data);
  }
}



bool Renderer::CullEntity(SDL_Point topleft, Sprite *sprite, LocationComponent *location)
{
  SDL_Rect entityRect;

  entityRect.x = static_cast<int>(location->GetX());
  entityRect.y = static_cast<int>(location->GetY());
  entityRect.w = sprite->GetLocation().w;
  entityRect.h = sprite->GetLocation().h;

  SDL_Rect screen = { topleft.x, topleft.y, m_windowSize.first, m_windowSize.second };

  return !SDL_HasIntersection(&entityRect, &screen); // if screen and entityRect do not intersect, cull entity as it's not being seen anyway
}

void Renderer::SortEntitiesByDrawPriority(std::vector<SpriteData> &drawData)
{
  std::stable_sort(drawData.begin(), drawData.end(), [&](SpriteData first, SpriteData second)
  {
    return (first.sprite->GetDrawPriority() < second.sprite->GetDrawPriority());
  }
  );
}

void Renderer::PerformEntityDraw(std::vector<SpriteData> drawdata, SDL_Point topleft)
{
  for (auto data : drawdata)
  {
    SDL_Texture *texture = m_spriteManager.GetSpriteSheet(data.sprite->GetSpriteSheetID());

    SDL_Rect locationOnScreen = data.sprite->GetLocation(); // need w/h-values. XY-values must be updated though
    locationOnScreen.x = data.location.x - topleft.x;
    locationOnScreen.y = data.location.y - topleft.y;
    SDL_Rect locationOnSpriteSheet = data.sprite->GetLocation();
    SDL_RendererFlip flip = data.flip ? SDL_FLIP_HORIZONTAL :  SDL_FLIP_NONE;

    SDL_RenderCopyEx(m_renderer, texture, &locationOnSpriteSheet, &locationOnScreen, 0, nullptr, flip);
  }
}

void Renderer::DrawWindows(const std::deque<std::unique_ptr<Window>> &windows)
{
  for (auto iter = windows.rbegin(); iter != windows.rend(); ++iter)
  {
    if (!(*iter)->IsActive())
    {
      continue;
    }
    DrawWindow((*iter)->GetTextures());
  }
}

void Renderer::DrawWindow(std::vector<std::pair<SDL_Rect, SDL_Texture *>> windowTextures)
{
  for (auto texture : windowTextures)
  {
    SDL_RenderCopy(m_renderer, texture.second, nullptr, &(texture.first));
  }
}

void Renderer::DebugDrawCollisionBoxes( EntityVector &entities, 
                                       EntityVector &staticCollidables, 
                                       SDL_Point topleft )
{
 
  for (const auto &entity : entities)
  {
    DrawSingleCollisionBox(entity, topleft);
  }

  for (const auto &entity : staticCollidables)
  {
    DrawSingleCollisionBox(entity, topleft);
  }


}


void Renderer::DrawSingleCollisionBox( const std::unique_ptr<Entity> &entity, SDL_Point &topleft )
{
   SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
  auto collisionComponent = static_cast<CollisionComponent *>(entity->GetComponent(ComponentType::COLLISION));
  if (collisionComponent == nullptr)
  {
    return;
  }
  auto hitboxes = collisionComponent->GetHitboxes(HitboxType::SOLID);

  for (auto hitbox : hitboxes)
  {
    hitbox.x -= topleft.x;
    hitbox.y -= topleft.y;

    SDL_RenderDrawRect(m_renderer, &hitbox);
  }

  hitboxes = collisionComponent->GetHitboxes(HitboxType::TRIGGER);
   SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 255);
  for (auto hitbox : hitboxes)
  {
    hitbox.x -= topleft.x;
    hitbox.y -= topleft.y;

    SDL_RenderDrawRect(m_renderer, &hitbox);
  }
}


MessageHandling Renderer::HandleTiledSpriteCreation(Message *message)
{
  auto msg = static_cast<NewTiledSpriteMessage *>(message);
  m_spriteManager.CreateTiledSprite(this, msg->GetBaseID(), msg->GetNewID(), msg->GetWidth(), msg->GetHeight());
  return MessageHandling::STOP_HANDLING;
}

void Renderer::DrawHud( Hud &hud )
{
  std::vector<std::pair<int, SDL_Point>> hudComponents = hud.GetTextureIdsAndPositions(m_windowSize.first, m_windowSize.second);
  for (auto component : hudComponents)
  {
    auto sprite = m_spriteManager.GetSprite(component.first);
    SDL_assert_release(sprite != nullptr);

    SDL_Rect locationOnSpriteSheet =  sprite->GetLocation();
    SDL_Rect locationOnScreen = locationOnSpriteSheet;
    locationOnScreen.x = component.second.x;
    locationOnScreen.y = component.second.y;
    
    SDL_RenderCopy(m_renderer, m_spriteManager.GetSpriteSheet(sprite->GetSpriteSheetID()), &locationOnSpriteSheet, &locationOnScreen);
  }


}
