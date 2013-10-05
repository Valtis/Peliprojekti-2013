#include "Entity/Entity.h"


void Entity::HandleMessages()
{
  while (!m_messages.empty())
  {
    HandleSingleMessage(std::move(m_messages.front()));
    m_messages.pop();
  }
}

void Entity::HandleSingleMessage(std::unique_ptr<Message> message)
{
  auto handlers =  m_messageHandlers[message->GetType()];

  if (handlers.empty())
  {
    SendMessageToParent(std::move(message));
    return;
  }

  PassMessageToHandlers(handlers, message);
  return;
}

void Entity::SendMessageToParent(std::unique_ptr<Message> message)
{
  if (m_parent != nullptr)
  {
    m_parent->AddMessage(std::move(message));
  }
}


void Entity::PassMessageToHandlers( std::vector<MessageCallback> handlers, std::unique_ptr<Message> &message )
{
  for (auto &handler : handlers)
  {
    if (!handler(message.get()))
    {
      return;
    }
  }
}
