#pragma once

#include "Message/Message.h"
#include <memory>
#include <map>
#include <vector>
#include <utility>

class VMState;
class MessageProcessor
{
public:
  ~MessageProcessor() { }

  // Passes messages to message handlers. If message was handled, returns true, else return false
  bool SendMessage(Message *message);
  // sends message to parent. Returns true if message was handled, false otherwise
  bool SendMessageUpwards(Message *message);

  // register new message handler for messages with MessageType and with given priority
  void RegisterMessageHandler(MessageType type, Priority priority, MessageCallback callback);
  void RegisterScriptMessageHandler(VMState *state, int type, int priority, std::string scriptName);

  void SetParent(MessageProcessor *parent) { m_parent = parent; }



protected:
  MessageProcessor() : m_parent(nullptr) { }


private:
  MessageProcessor *m_parent;
  std::vector<MessageProcessor *> m_children;
  typedef std::pair<Priority, MessageCallback> PrioritizedCallback;

  void PassMessageToHandlers(const std::vector<PrioritizedCallback> &handlers, Message *message );
  void SortHandlers(MessageType type);

  std::map<MessageType, std::vector<PrioritizedCallback>> m_messageHandlers;

};
