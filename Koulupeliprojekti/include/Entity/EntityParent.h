#pragma once

#include <memory>
#include "Message/Message.h"

// abstract class for Message handling
class EntityParent
{
public:
  virtual void SendMessage(Message *message) = 0;
  virtual void RegisterMessageHandler(MessageType type, Priority priority, MessageCallback callback) = 0;

protected:

private:

};