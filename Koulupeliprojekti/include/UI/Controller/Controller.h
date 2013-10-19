#pragma once
#include <utility>
#include <unordered_map>
#include "UI/Commands/Command.h"

class Controller
{
public:
	Controller() {}
	virtual ~Controller() {}
	virtual Command* HandleInput() = 0;
};
