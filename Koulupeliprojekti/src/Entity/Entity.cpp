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