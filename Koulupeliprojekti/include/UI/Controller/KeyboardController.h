#pragma once
#include <SDL.h>
#include <queue>
#include "UI/Controller/Controller.h"
#include "Message/Commands/Command.h"

class KeyboardController : public Controller
{
public:
	Command* HandleInput(SDL_Event event) const override;
};
