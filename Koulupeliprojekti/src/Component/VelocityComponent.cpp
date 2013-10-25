#include "Component/VelocityComponent.h"
#include "Entity/Entity.h"
#include "Utility/LoggerManager.h"
#include "Message/VelocityChangeMessage.h"

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