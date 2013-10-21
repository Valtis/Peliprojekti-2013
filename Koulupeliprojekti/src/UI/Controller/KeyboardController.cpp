#include <SDL.h>
#include <iostream>
#include "UI/Controller/KeyboardController.h"
#include "Message/Commands/KeyboardCommand.h"

KeyboardController::KeyboardController()
{
}

Command* KeyboardController::HandleInput(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
		return new KeyboardCommand(event);
	else return nullptr;
}
