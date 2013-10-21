#pragma once
#include <SDL.h>
#include <queue>
#include <memory>
#include "Message/Commands/Command.h"
#include "UI/Controller/Controller.h"

class MouseController : public Controller
{
public:
	MouseController();
	Command* HandleInput(SDL_Event event) override;
};
