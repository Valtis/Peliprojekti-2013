#include "Component/LocationComponent.h"
#include "Entity/Entity.h"
#include "Utility/LoggerManager.h"
#include "Message/LocationChangeMessage.h"
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
  if (msg->GetType() != MessageType::LOCATION_CHANGE)
  {
    LoggerManager::GetLog(COMPONENT_LOG).AddLine(LogLevel::WARNING, "Invalid message type received in LocationComponent::HandleLocationChangeMessage() - ignoring");
    return true;
  }

  auto *locationMessage = static_cast<LocationChangeMessage *>(msg);
  
  m_x += locationMessage->GetXChange();
  m_y += locationMessage->GetYChange();
  return false;
}