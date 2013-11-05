#include "Component/VelocityComponent.h"
#include "Entity/Entity.h"
#include "Utility/LoggerManager.h"
#include "Message/VelocityChangeMessage.h"
#include "Message/SetVelocityMessage.h"

VelocityComponent::VelocityComponent() : m_xVelocity(0), m_yVelocity(0)
{

}

VelocityComponent::~VelocityComponent()
{

}

void VelocityComponent::Update(double ticksPassed)
{
  if (m_xVelocity != 0 || m_yVelocity != 0)
  {
    auto locationChangeMessage = MessageFactory::CreateLocationChangeMessage(m_xVelocity*ticksPassed, m_yVelocity*ticksPassed);
    GetOwner()->SendMessage(locationChangeMessage.get());
  }
}

void VelocityComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::VELOCITY_CHANGE, Priority::NORMAL, 
    [&](Message *msg) { return this->HandleVelocityChangeMessage(msg); });

  GetOwner()->RegisterMessageHandler(MessageType::SET_VELOCITY, Priority::NORMAL, 
    [&](Message *msg) { return this->HandleSetVelocityMessage(msg); });
}

bool VelocityComponent::HandleVelocityChangeMessage(Message *msg)
{
  if (msg->GetType() != MessageType::VELOCITY_CHANGE)
  {
    LoggerManager::GetLog(COMPONENT_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in VelocityComponent::HandleVelocityChangeMessage() - ignoring");
    return true;
  }

  auto *velocityMessage = static_cast<VelocityChangeMessage *>(msg);

  m_xVelocity += velocityMessage->GetXChange();
  m_yVelocity += velocityMessage->GetYChange();
  return false;
}

bool VelocityComponent::HandleSetVelocityMessage(Message *msg)
{
  if (msg->GetType() != MessageType::SET_VELOCITY)
  {
    LoggerManager::GetLog(COMPONENT_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in VelocityComponent::HandleSetVelocityMessage() - ignoring");
    return true;
  }

  auto *velocityMessage = static_cast<SetVelocityMessage *>(msg);

  if (velocityMessage->GetDirection() == Velocity::XY)
  {
    m_xVelocity = velocityMessage->GetXVelocity();
    m_yVelocity = velocityMessage->GetYVelocity();
  }
  else if (velocityMessage->GetDirection() == Velocity::X)
  {
    m_xVelocity = velocityMessage->GetXVelocity();
  }
  else
  {
    m_yVelocity = velocityMessage->GetYVelocity();
  }
  return false;
}
