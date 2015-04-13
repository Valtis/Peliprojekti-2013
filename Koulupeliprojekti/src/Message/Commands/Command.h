#pragma once
#include "Message/Message.h"

class Command : public Message
{
public:
	virtual ~Command() { }
	virtual MessageType GetType() const = 0;
protected:
	Command() { }
private:
};
