#include "Component/VelocityComponent.h"

#include "Utility/LoggerManager.h"
#include "Utility/GenericDefines.h"
#include "Message/VelocityChangeMessage.h"
#include "Message/SetVelocityMessage.h"
#include "Message/MessageFactory.h"
#include "Entity/Entity.h"
VelocityComponent::VelocityComponent(double x, double y) : m_currentXVelocity(0), m_currentYVelocity(0), m_xChangeVelocity(x), m_yChangeVelocity(y)
{

}

VelocityComponent::~VelocityComponent()
{

}

void VelocityComponent::Update(double ticksPassed)
{
  if (m_currentXVelocity != 0 || m_currentYVelocity != 0)
  {
    auto locationChangeMessage = MessageFactory::CreateLocationChangeMessage(m_currentXVelocity*ticksPassed, m_currentYVelocity*ticksPassed);
    GetOwner()->SendMessage(locationChangeMessage.get());
  }

  // shouldn't really be hardcoded here but eh..
  if (m_currentXVelocity == 0)
  {
    auto animationMsg = MessageFactory::CreateChangeAnimationMessage(IDLE_ANIMATION); 
    GetOwner()->SendMessage(animationMsg.get());
  }
  else if (m_currentXVelocity != 0)
  {
    auto animationMsg = MessageFactory::CreateChangeAnimationMessage(WALKING_ANIMATION); 
    GetOwner()->SendMessage(animationMsg.get());
  }

}

void VelocityComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::SET_VELOCITY, Priority::NORMAL, 
    [&](Message *msg) { return this->HandleSetVelocityMessage(msg); });
}

MessageHandling VelocityComponent::HandleSetVelocityMessage(Message *msg)
{
  if (msg->GetType() != MessageType::SET_VELOCITY)
  {
    LoggerManager::GetLog(COMPONENT_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in VelocityComponent::HandleSetVelocityMessage() - ignoring");
    return MessageHandling::PASS_FORWARD;
  }

  auto *velocityMessage = static_cast<SetVelocityMessage *>(msg);

  if (velocityMessage->GetXDirection() == Direction::LEFT)
  {
    m_currentXVelocity = -m_xChangeVelocity;
  }
  else if (velocityMessage->GetXDirection() == Direction::RIGHT)
  {
    m_currentXVelocity = m_xChangeVelocity;
  }
  else if (velocityMessage->GetXDirection() == Direction::NONE)
  {
    m_currentXVelocity = 0;
  }

  if (velocityMessage->GetYDirection() == Direction::UP)
  {
    m_currentYVelocity = -m_yChangeVelocity;
  }
  else if (velocityMessage->GetYDirection() == Direction::DOWN)
  {
    m_currentYVelocity = m_yChangeVelocity;
  }
  else if (velocityMessage->GetYDirection() == Direction::NONE)
  {
    m_currentYVelocity = 0;
  }
  return MessageHandling::STOP_HANDLING;
}
