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

}

void VelocityComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::VELOCITY_CHANGE, Priority::NORMAL, 
    [&](Message *msg) { return this->HandleVelocityChangeMessage(msg); });
}

void VelocityComponent::HandleVelocityChangeMessage(Message *msg)
{

}