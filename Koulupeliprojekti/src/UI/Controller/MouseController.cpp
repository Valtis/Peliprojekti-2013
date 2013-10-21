#include <SDL.h>
#include <string>
#include <memory>
#include <utility>
#include <iostream>
#include "UI/Controller/MouseController.h"
#include "Message/Commands/Command.h"
#include "Message/Commands/MouseCommand.h"
#include "Utility/LoggerManager.h"

Command* MouseController::HandleInput(SDL_Event event) const
{
	if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
		return new MouseCommand(event);
	else
		return nullptr;
}
