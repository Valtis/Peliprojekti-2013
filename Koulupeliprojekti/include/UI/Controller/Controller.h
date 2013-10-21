#pragma once
#include <SDL.h>
#include "Message/Commands/Command.h"

class Controller
{
public:
	Controller() {}
	virtual ~Controller() {}
	virtual Command* HandleInput(SDL_Event event) = 0;
};
