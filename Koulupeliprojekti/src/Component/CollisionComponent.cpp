#include "Component/CollisionComponent.h"
#include "Component/LocationComponent.h"
#include "Entity/Entity.h"
#include "Message/CollisionMessage.h"
#include "Utility/LoggerManager.h"

CollisionComponent::CollisionComponent()
{
  m_hitbox.h = 0;
  m_hitbox.w = 0;
  m_hitbox.x = 0;
  m_hitbox.y = 0;
}

CollisionComponent::CollisionComponent(int w, int h)
{
  m_hitbox.h = w;
  m_hitbox.w = h;
  m_hitbox.x = 0;
  m_hitbox.y = 0;
}

CollisionComponent::CollisionComponent(int x, int y, int w, int h)
{
  m_hitbox.h = h;
  m_hitbox.w = w;
  m_hitbox.x = x;
  m_hitbox.y = y;
}

CollisionComponent::~CollisionComponent()
{

}

void CollisionComponent::OnAttatchingToEntity()
{
  Component::OnAttatchingToEntity();
  
  // GetOwner()->RegisterMessageHandler(MessageType::COLLISION, Priority::NORMAL, 
  // [&](Message *msg) { return this->HandleCollisionMessage(msg); });
}

SDL_Rect CollisionComponent::GetHitbox()
{
  SDL_Rect rect;
  LocationComponent *loc;
  
  loc =
    static_cast<LocationComponent *>(GetOwner()->GetComponent(ComponentType::LOCATION));
  
  rect.w = m_hitbox.w;
  rect.h = m_hitbox.h;
  rect.x = m_hitbox.x + static_cast<int>(loc->GetX() + 0.5);
  rect.y = m_hitbox.y + static_cast<int>(loc->GetY() + 0.5);

  return rect;
}

bool CollisionComponent::HandleCollisionMessage(Message *msg)
{
  if (msg->GetType() != MessageType::COLLISION)
  {
    LoggerManager::GetLog(COMPONENT_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in CollisionComponent::HandleCollisionMessage() - ignoring");
    return true;
  }

  return false;
}
