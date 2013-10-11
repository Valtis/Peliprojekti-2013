#pragma once

#include <memory>
#include "Message/Message.h"

// interface class for entity parents. Entity/level/other classes that can act as parents for entities must inherit this
class EntityParent
{
public:
  virtual void SendMessage(Message *message) = 0;
  virtual void RegisterMessageHandler(MessageType type, Priority priority, MessageCallback callback) = 0;

protected:

private:

};
