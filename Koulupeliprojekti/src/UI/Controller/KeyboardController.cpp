#include <SDL.h>
#include <iostream>
#include "UI/Controller/KeyboardController.h"
#include "Message/Commands/KeyboardCommand.h"

Command* KeyboardController::HandleInput(SDL_Event event) const
{
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
		return new KeyboardCommand(event);
	else return nullptr;
}
