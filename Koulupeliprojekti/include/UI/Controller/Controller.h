#pragma once
#include "Message/Commands/Command.h"

class Controller
{
public:
	Controller() {}
	virtual ~Controller() {}
	virtual Command* HandleInput() = 0;
};
