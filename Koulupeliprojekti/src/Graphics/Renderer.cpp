#include "Graphics/Renderer.h"
#include "Graphics/SpriteManager.h"
#include "Graphics/Sprite.h"
#include "Graphics/Camera/Camera.h"
#include "Component/GraphicsComponent.h"
#include "Component/LocationComponent.h"
// collision is for debugging - can be removed
#include "Component/CollisionComponent.h"
#include "UI/Window/Window.h"
#include "Entity/Entity.h"

#include <stdexcept>
#include <string>
#include <algorithm>



Renderer::Renderer()
{

}

Renderer::~Renderer()
{
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
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

void Renderer::LoadSprites(std::string datafilePath)
{
  m_spriteManager.Initialize(this, datafilePath);
}

void Renderer::Draw(Camera *camera, const std::vector<std::unique_ptr<Entity>> &entities, const std::deque<std::unique_ptr<Window>> &windows)
{
  SDL_assert(camera != nullptr);
  ClearScreen();
  DrawEntities(camera, entities);
  DrawWindows(windows);
  SDL_RenderPresent(m_renderer);
}


void Renderer::ClearScreen()
{
  SDL_SetRenderDrawColor(m_renderer, 174, 234, 255, 255);
  SDL_RenderClear(m_renderer);
}

void Renderer::DrawEntities(Camera *camera, const std::vector<std::unique_ptr<Entity>> &entities)
{
  SDL_Point topleft;
  topleft.x = camera->GetX() - m_windowSize.first/2;
  topleft.y = camera->GetY() - m_windowSize.second/2;

  std::vector<std::pair<SDL_Point, Sprite *>> spriteIds = GetDataForDrawing(topleft, entities);
  SortEntitiesByDrawPriority(spriteIds);
  PerformEntityDraw(spriteIds, topleft);

  //DebugDrawCollisionBoxes(entities, topleft);

}

std::vector<std::pair<SDL_Point, Sprite *>> Renderer::GetDataForDrawing(SDL_Point topleft, const std::vector<std::unique_ptr<Entity>> &entities)
{
  std::vector<std::pair<SDL_Point, Sprite *>> retval;
  for (auto &entity : entities)
  {
    auto location = static_cast<LocationComponent *>(entity->GetComponent(ComponentType::LOCATION));
    auto graphics = static_cast<GraphicsComponent *>(entity->GetComponent(ComponentType::GRAPHICS));
    SDL_assert(location != nullptr);
    SDL_assert(graphics != nullptr);

    Sprite *sprite = m_spriteManager.GetSprite(graphics->GetSpriteID());
    SDL_assert(sprite != nullptr);

    if (CullEntity(topleft, sprite, location))
    {
      continue;
    }

    SDL_Point loc = { (int)location->GetX(), (int)location->GetY() };
    retval.push_back(std::make_pair(loc, sprite));
  }
  return retval;
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

void Renderer::SortEntitiesByDrawPriority(std::vector<std::pair<SDL_Point, Sprite *>> &drawData)
{
  std::stable_sort(drawData.begin(), drawData.end(), [&](std::pair<SDL_Point, Sprite *> first, std::pair<SDL_Point, Sprite *> second)
  {
    return (first.second->GetDrawPriority() < second.second->GetDrawPriority());
  }
  );
}

void Renderer::PerformEntityDraw(std::vector<std::pair<SDL_Point, Sprite *>> drawdata, SDL_Point topleft)
{
  for (auto data : drawdata)
  {
    SDL_Texture *texture = m_spriteManager.GetSpriteSheet(data.second->GetSpriteSheetID());

    SDL_Rect locationOnScreen = data.second->GetLocation(); // need w/h-values. XY-values must be updated though
    locationOnScreen.x = data.first.x - topleft.x;
    locationOnScreen.y = data.first.y - topleft.y;
    SDL_Rect loc = data.second->GetLocation();

    SDL_RenderCopy(m_renderer, texture, &loc, &locationOnScreen);
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

void Renderer::DebugDrawCollisionBoxes( const std::vector<std::unique_ptr<Entity>> &entities, SDL_Point topleft )
{

    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
    for (auto &entity : entities)
    {

    auto collisionComponent = static_cast<CollisionComponent *>(entity->GetComponent(ComponentType::COLLISION));
    if (collisionComponent == nullptr)
    {
      return;
    }
    auto hitboxes =  collisionComponent->GetHitboxes(HitboxType::OBJECT);

    for (auto hitbox : hitboxes)
    {
      hitbox.x -= topleft.x;
      hitbox.y -= topleft.y;

      SDL_assert(SDL_RenderDrawRect(m_renderer, &hitbox) == 0);
    }
  }
}
