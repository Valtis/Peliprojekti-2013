#include "Message/MessageProcessor.h"
#include <algorithm>
bool MessageProcessor::SendMessage(Message *message)
{
  auto handlers =  m_messageHandlers[message->GetType()];
  if (handlers.empty())
  {
    if (m_parent == nullptr)
    {
      return false;
    }
    else
    {
      return m_parent->SendMessage(message);
    }
  }

  PassMessageToHandlers(handlers, message);
  return true;
}

void MessageProcessor::PassMessageToHandlers(const std::vector<PrioritizedCallback> &handlers, Message *message )
{
  for (auto &handler : handlers)
  {
    if (handler.second(message) == MessageHandling::STOP_HANDLING)
    {
      return;
    }
  }
}


void MessageProcessor::RegisterMessageHandler(MessageType type, Priority priority, MessageCallback callback)
{
  m_messageHandlers[type].push_back(std::make_pair(priority, callback));
  SortHandlers(type);
}

void MessageProcessor::SortHandlers(MessageType type)
{
  // should be sorted from highest to lowest
  std::stable_sort(m_messageHandlers[type].begin(), m_messageHandlers[type].end(), 
    [](PrioritizedCallback firstCallback, PrioritizedCallback secondCallback) { return static_cast<int>(firstCallback.first) >= static_cast<int>(secondCallback.first) ; }
  );
}
