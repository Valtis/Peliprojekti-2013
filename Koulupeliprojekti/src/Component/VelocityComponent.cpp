#include "Component/VelocityComponent.h"
#include "Entity/Entity.h"

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
  return true;
}