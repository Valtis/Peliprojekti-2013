#pragma once

#include <memory>
#include "Message/Message.h"

// interface for classes that can be parents for entities
class EntityParent
{
public:
	virtual void AddMessage(std::unique_ptr<Message> message) = 0;
  virtual void RegisterMessageHandler(MessageType type, MessageCallback callback) = 0;
};