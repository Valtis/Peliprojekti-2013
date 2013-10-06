#pragma once
#include <memory>
#include <map>
#include <vector>
#include <utility>

#include "Message/Message.h"
class MessageProcessor
{
public:
  MessageProcessor() { }
  ~MessageProcessor() { }

  // Passes messages to message handlers. If message was handled, returns true, else return false
  virtual bool SendMessage(Message *message);
  // register new message handler for messages with MessageType and with given priority
  virtual void RegisterMessageHandler(MessageType type, Priority priority, MessageCallback callback);
private:

  typedef std::pair<Priority, MessageCallback> PrioritizedCallback;

  void PassMessageToHandlers(const std::vector<PrioritizedCallback> &handlers, Message *message );
  void SortHandlers(MessageType type);

  std::map<MessageType, std::vector<PrioritizedCallback>> m_messageHandlers;
  
};