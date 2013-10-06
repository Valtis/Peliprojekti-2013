#include "Entity/Entity.h"

void Entity::RegisterMessageHandler(MessageType type, Priority priority, MessageCallback callback)
{
  m_messageProcessor.RegisterMessageHandler(type, priority, callback);
}

void Entity::SendMessage(Message *message)
{
  if (!m_messageProcessor.SendMessage(message))
  {
    SendMessageToParent(message);
  }
}

void Entity::SendMessageToParent(Message *message)
{
  if (m_parent != nullptr)
  {
    m_parent->SendMessage(message);
  }
}


void Entity::SendMessageToChildren(Message *message)
{
  for (auto &child : m_children)
  {
    child->SendMessage(message);
  }
}


void Entity::AddComponent(ComponentType type, std::unique_ptr<Component> c)
{
  if (type != ComponentType::NONE)
  {
    m_components[type] = std::move(c);
  }
}

Component *Entity::GetComponent(ComponentType type)
{
  if (m_components.count(type) == 0)
  {
    return nullptr;
  }

  return m_components[type].get();
}

void Entity::Update(double ticksPassed)
{
  for (auto &component : m_components)
  {
    component.second->Update(ticksPassed);
  }
}