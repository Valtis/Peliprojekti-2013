#include "Message/MessageProcessor.h"
#include "VM/Core/VM.h"
#include <algorithm>
bool MessageProcessor::SendMessage(Message *message)
{
  auto handlers =  m_messageHandlers[message->GetType()];
  if (handlers.empty())
  {
    return SendMessageUpwards(message);
  }
  PassMessageToHandlers(handlers, message);
  return true;
}

bool MessageProcessor::SendMessageUpwards(Message *message)
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


void MessageProcessor::RegisterScriptMessageHandler(VMState *state, int type, int priority, std::string scriptName) {
  MessageCallback callback = [=](Message *message) -> MessageHandling {
    auto messageHandling = VMInstance().InvokeFunction(*state, scriptName, { VMValue{ message } });
    try {
      return static_cast<MessageHandling>(messageHandling.AsInt());
    } catch (std::exception &ex) {
      throw std::runtime_error(std::string("An error occurred when parsing script message handler return value: ") + ex.what());
    }
  };

  m_messageHandlers[static_cast<MessageType>(type)].push_back({ static_cast<Priority>(priority), callback });
  SortHandlers(static_cast<MessageType>(type));
}

void MessageProcessor::RegisterMessageHandler(MessageType type, Priority priority, MessageCallback callback)
{
  m_messageHandlers[type].push_back({ priority, callback });
  SortHandlers(type);
}

void MessageProcessor::SortHandlers(MessageType type)
{
  // should be sorted from highest to lowest
  std::stable_sort(m_messageHandlers[type].begin(), m_messageHandlers[type].end(), 
    [](PrioritizedCallback firstCallback, PrioritizedCallback secondCallback) { return static_cast<int>(firstCallback.first) >= static_cast<int>(secondCallback.first) ; }
  );
}
