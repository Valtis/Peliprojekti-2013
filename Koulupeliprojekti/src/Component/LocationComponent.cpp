#include "Component/LocationComponent.h"
#include "Entity/Entity.h"

LocationComponent::LocationComponent() : m_x(0), m_y(0), m_rotation(0)
{

}

LocationComponent::~LocationComponent()
{

}

void LocationComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::LOCATION_CHANGE, Priority::NORMAL, 
    [&](Message *msg) { return this->HandleLocationChangeMessage(msg); });
}

bool LocationComponent::HandleLocationChangeMessage(Message *msg)
{
  return true;
}