#include "Component/CollisionComponent.h"
#include "Message/CollisionMessage.h"
#include "Message/QueryLocationMessage.h"
#include "Message/MessageFactory.h"
#include "Utility/LoggerManager.h"
#include "Entity/Entity.h"
CollisionComponent::CollisionComponent()
{

}

CollisionComponent::~CollisionComponent()
{

}

void CollisionComponent::AddHitbox(SDL_Rect &box, const HitboxType type)
{
  switch (type)
  {
  case HitboxType::SOLID:
    m_object_hitboxes.push_back(box);
    break;
  case HitboxType::TRIGGER:
    m_trigger_hitboxes.push_back(box);
    break;
  default:
    LoggerManager::GetLog("error.log").AddLine(LogLevel::WARNING,
                                               "Unrecognized Hitbox Type.");
  }
}

void CollisionComponent::AddHitbox(int x, int y, int w, int h,
                                   const HitboxType type)
{
  SDL_Rect box = { x, y, w, h };
  AddHitbox(box, type);
}

const std::vector<SDL_Rect> CollisionComponent::GetHitboxes(const HitboxType type)
{
  std::vector<SDL_Rect> boxes;
  
  switch (type)
  {
  case HitboxType::SOLID:
    for (SDL_Rect box : m_object_hitboxes)
      boxes.push_back(TransformHitbox(box));
    break;
  case HitboxType::TRIGGER:
    for (SDL_Rect box : m_trigger_hitboxes)
      boxes.push_back(TransformHitbox(box));
    break;
  }

  return boxes;
}

SDL_Rect CollisionComponent::TransformHitbox(const SDL_Rect box)
{
  SDL_Rect rect;
  auto queryMsg = MessageFactory::CreateQueryLocationMessage();
  bool answered = GetOwner()->SendMessage(queryMsg.get());
 
  if (!answered)
  {
    throw std::runtime_error("No location component on entity with collision component\n");
  }
  
  rect.w = box.w;
  rect.h = box.h;
  rect.x = box.x + static_cast<int>(queryMsg->GetX());
  rect.y = box.y + static_cast<int>(queryMsg->GetY());

  return rect;
}
