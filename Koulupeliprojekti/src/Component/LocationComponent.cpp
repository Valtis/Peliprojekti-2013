#include "Component/LocationComponent.h"
#include "Utility/LoggerManager.h"

#include "Message/LocationChangeMessage.h"
#include "Message/SetLocationMessage.h"
#include "Message/FiringDirectionMessage.h"
#include "Message/CollisionMessage.h"
#include "Message/QueryLocationMessage.h"
#include "Message/QueryCanIJumpMessage.h"

#include "Component/FactionComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/VelocityComponent.h"

#include "Entity/Entity.h"

LocationComponent::LocationComponent() : m_x(0), m_y(0), 
  m_direction(Direction::RIGHT), m_firingDirection(Direction::RIGHT)
{

}

LocationComponent::LocationComponent(double x, double y) : m_x(x), m_y(y),
  m_direction(Direction::RIGHT)
{

}


LocationComponent::~LocationComponent()
{

}

void LocationComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::LOCATION_CHANGE, Priority::LOWEST, 
    [=](Message *msg) { return this->HandleLocationChangeMessage(msg); });
  GetOwner()->RegisterMessageHandler(MessageType::COLLISION, Priority::LOWEST, 
    [=](Message *msg) { return this->HandleCollisionMessage(msg); });
  GetOwner()->RegisterMessageHandler(MessageType::SET_LOCATION, Priority::LOWEST, 
    [=](Message *msg) { return this->HandleSetLocationMessage(msg); });
  GetOwner()->RegisterMessageHandler(MessageType::FIRE_DIRECTION, Priority::LOWEST, 
    [=](Message *msg) { return this->HandleSetFiringDirectionMessage(msg); });
  GetOwner()->RegisterMessageHandler(MessageType::QUERY_LOCATION, Priority::LOWEST, 
    [=](Message *msg) { return this->HandleQueryLocationMessage(msg); });
  GetOwner()->RegisterMessageHandler(MessageType::QUERY_JUMP, Priority::LOWEST, 
    [=](Message *msg) { return this->HandleQueryLocationMessage(msg); });
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
    LoggerManager::GetLog(COMPONENT_LOG).AddLine(LogLevel::WARNING, 
      "Invalid message type received in LocationComponent::HandleLocationChangeMessage() - ignoring");
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

MessageHandling LocationComponent::HandleSetLocationMessage(Message *msg)
{
  if (msg->GetType() != MessageType::SET_LOCATION)
  {
    LoggerManager::GetLog(COMPONENT_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in LocationComponent::HandleSetLocationMessage() - ignoring");
    return MessageHandling::PASS_FORWARD;
  }

  auto locMsg = static_cast<SetLocationMessage *>(msg);
  m_x = locMsg->GetX();
  m_y = locMsg->GetY();
  return MessageHandling::STOP_HANDLING;
}

MessageHandling LocationComponent::HandleQueryLocationMessage(Message *msg)
{
  if (msg->GetType() != MessageType::QUERY_JUMP)
  {
    LoggerManager::GetLog(COMPONENT_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in LocationComponent::HandleQueryLocationMessage() - ignoring");
    return MessageHandling::PASS_FORWARD;
  }

  auto queryMsg = static_cast<QueryLocationMessage *>(msg);
  queryMsg->SetXY(m_x, m_y);

  return MessageHandling::STOP_HANDLING;
}

MessageHandling LocationComponent::HandleCanIJumpMessage(Message *msg)
{
  auto queryJumpMsg = static_cast<QueryCanIJumpMessage *>(msg);
  queryJumpMsg->SetJump(m_collision[Direction::DOWN]);
  return MessageHandling::STOP_HANDLING;
}



MessageHandling LocationComponent::HandleCollisionMessage(Message *msg)
{
  CollisionMessage *colMsg = static_cast<CollisionMessage *>(msg);
  if (colMsg->GetHitType() != HitboxType::SOLID)
    return MessageHandling::PASS_FORWARD;

  VelocityComponent *v =
    static_cast<VelocityComponent *>(GetOwner()->GetComponent(ComponentType::VELOCITY));
  if (v == nullptr)
    return MessageHandling::PASS_FORWARD;

  FactionComponent *myFac =
    static_cast<FactionComponent *>(GetOwner()->GetComponent(ComponentType::FACTION));
  MessageHandling handling = MessageHandling::STOP_HANDLING;
  for (auto collider : colMsg->GetEntities())
  {
    FactionComponent *colFac = 
      static_cast<FactionComponent *>(collider->GetComponent(ComponentType::FACTION));

    if (myFac == nullptr || colFac == nullptr ||
      myFac->GetFaction() != colFac->GetFaction())
      handling = MessageHandling::PASS_FORWARD;
  }
  Direction h_side = colMsg->GetHorizontalSide();
  Direction v_side = colMsg->GetVerticalSide();

  if (m_collision[h_side] && m_collision[h_side])
    return handling;

  if (!m_collision[h_side] && colMsg->GetIntersection().h > 8)
  {
    if (h_side == Direction::LEFT)
      m_x += colMsg->GetPoint().x;
    else if (h_side == Direction::RIGHT)
      m_x -= colMsg->GetPoint().x;
  }

  if (!m_collision[v_side])
  {
    if (v_side == Direction::UP)
      m_y += colMsg->GetPoint().y;
    else if (v_side == Direction::DOWN)
      m_y -= colMsg->GetPoint().y;
  }

  m_collision[v_side] = true;
  m_collision[h_side] = true;

  return handling;
}

MessageHandling LocationComponent::HandleSetFiringDirectionMessage(Message *msg)
{

  auto fireMsg = static_cast<FiringDirectionMessage *>(msg);
  if (fireMsg->GetDirection() != Direction::UNCHANGED && fireMsg->GetDirection() != Direction::NONE)
  {  
    m_firingDirection = fireMsg->GetDirection();
  }
  return MessageHandling::STOP_HANDLING;
}