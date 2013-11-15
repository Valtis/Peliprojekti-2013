#include "Component/LocationComponent.h"
#include "Entity/Entity.h"
#include "Utility/LoggerManager.h"
#include "Message/LocationChangeMessage.h"
#include "Component/VelocityComponent.h"
#include "Message/CollisionMessage.h"
#include "Component/FactionComponent.h"

LocationComponent::LocationComponent() : m_x(0), m_y(0), m_direction(Direction::RIGHT)
{

}

LocationComponent::~LocationComponent()
{

}

void LocationComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::LOCATION_CHANGE, Priority::NORMAL, 
    [&](Message *msg) { return this->HandleLocationChangeMessage(msg); });
  GetOwner()->RegisterMessageHandler(MessageType::COLLISION, Priority::NORMAL, 
    [&](Message *msg) { return this->HandleCollisionMessage(msg); });

}

void LocationComponent::Update(double ticksPassed)
{
   for (auto &collision : m_collision)
   {
     collision.second = false;
   }
}

MessageHandling LocationComponent::HandleLocationChangeMessage(Message *msg)
{
  if (msg->GetType() != MessageType::LOCATION_CHANGE)
  {
    LoggerManager::GetLog(COMPONENT_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in LocationComponent::HandleLocationChangeMessage() - ignoring");
    return MessageHandling::PASS_FORWARD;
  }
  auto *locationMessage = static_cast<LocationChangeMessage *>(msg);

  m_x += locationMessage->GetXChange();
  m_y += locationMessage->GetYChange();

  if (locationMessage->GetXChange() > 0)
  {
    m_direction = Direction::RIGHT;
  }
  else if (locationMessage->GetXChange() < 0)
  {
    m_direction = Direction::LEFT;
  }

  return MessageHandling::STOP_HANDLING;
}

bool LocationComponent::CanIJump()
{
  return m_collision[CollisionSide::DOWN];
}

MessageHandling LocationComponent::HandleCollisionMessage(Message *msg)
{
  CollisionMessage *colMsg = static_cast<CollisionMessage *>(msg);
  VelocityComponent *v = static_cast<VelocityComponent *>(GetOwner()->GetComponent(ComponentType::VELOCITY));
  CollisionSide side;
  Entity *collider = colMsg->GetEntity();

  auto *myFac = static_cast<FactionComponent *>(GetOwner()->GetComponent(ComponentType::FACTION));
  auto *colFac = static_cast<FactionComponent *>(collider->GetComponent(ComponentType::FACTION));

  if (myFac != nullptr && colFac != nullptr && myFac->GetFaction() == colFac->GetFaction())
  {
    return MessageHandling::PASS_FORWARD;
  }

  if (v == nullptr)
    return MessageHandling::PASS_FORWARD;

  side = colMsg->GetSide();

  if (m_collision[side])
  {
    return MessageHandling::STOP_HANDLING;
  }

  m_collision[side] = true;

  if (side == CollisionSide::RIGHT)
    m_x -= colMsg->GetIntersection().w;
  else if (side == CollisionSide::LEFT)
    m_x += colMsg->GetIntersection().w;
  else if (side == CollisionSide::UP)
    m_y += colMsg->GetIntersection().h;
  else if (side == CollisionSide::DOWN)
    m_y -= colMsg->GetIntersection().h;
 // if (side == CollisionSide::DOWN)
  
  return MessageHandling::STOP_HANDLING;
}
