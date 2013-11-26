#include "Component/LocationComponent.h"
#include "Entity/Entity.h"
#include "Utility/LoggerManager.h"
#include "Message/LocationChangeMessage.h"
#include "Component/VelocityComponent.h"
#include "Message/CollisionMessage.h"
#include "Component/FactionComponent.h"
#include "Component/CollisionComponent.h"

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
  if (colMsg->GetHitType() != HitboxType::SOLID)
    return MessageHandling::PASS_FORWARD;

  fprintf(stdout,"h: %d, v: %d, w: %d, h: %d, t: %d\n",
      colMsg->GetHorizontalSide(),colMsg->GetVerticalSide(),
      colMsg->GetPoint().x,colMsg->GetPoint().y,
      colMsg->GetHitType());

  VelocityComponent *v =
    static_cast<VelocityComponent *>(GetOwner()->GetComponent(ComponentType::VELOCITY));
  if (v == nullptr)
    return MessageHandling::PASS_FORWARD;
  
  std::vector<Entity *> colliders = colMsg->GetEntities();

  FactionComponent *myFac =
    static_cast<FactionComponent *>(GetOwner()->GetComponent(ComponentType::FACTION));
  MessageHandling handling = MessageHandling::STOP_HANDLING;
  for (auto collider : colliders)
  {
    FactionComponent *colFac = 
      static_cast<FactionComponent *>(collider->GetComponent(ComponentType::FACTION));

    if (myFac == nullptr || colFac == nullptr ||
        myFac->GetFaction() != colFac->GetFaction())
      handling = MessageHandling::PASS_FORWARD;
  }
  CollisionSide h_side = colMsg->GetHorizontalSide();
  CollisionSide v_side = colMsg->GetVerticalSide();

  if (m_collision[h_side] && m_collision[h_side])
    return handling;

  double v_x = v->GetVelocityX();
  double v_y = v->GetVelocityY();
  double v_d = v_x + v_y;
  v_x /= v_d;
  v_y /= v_d;

  if (!m_collision[h_side])
  {
    if (h_side == CollisionSide::LEFT)
      m_x += colMsg->GetPoint().x;
    else if (h_side == CollisionSide::RIGHT)
      m_x -= colMsg->GetPoint().x;
  }

  if (!m_collision[v_side])
  {
    if (v_side == CollisionSide::UP)
      m_y += colMsg->GetPoint().y;
    else if (v_side == CollisionSide::DOWN)
      m_y -= colMsg->GetPoint().y;
  }

  m_collision[v_side] = true;
  m_collision[h_side] = true;

  return handling;
}
