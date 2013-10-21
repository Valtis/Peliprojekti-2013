#include <SDL.h>
#include "UI/Controller/KeyboardController.h"
#include "Message/Commands/KeyboardCommand.h"

Command* KeyboardController::HandleInput(SDL_Event event) const
{
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	{
		bool state = (event.type == SDL_KEYDOWN);
		Command* cmd = nullptr;
		switch (event.key.keysym.sym)
		{
		case SDLK_LEFT:
			cmd = new KeyboardCommand(KeyCommand::LEFT,state);
			break;
		case SDLK_RIGHT:
			cmd = new KeyboardCommand(KeyCommand::RIGHT,state);
			break;
		case SDLK_UP:
			cmd = new KeyboardCommand(KeyCommand::UP,state);
			break;
		case SDLK_DOWN:
			cmd = new KeyboardCommand(KeyCommand::DOWN,state);
			break;
		case SDLK_SPACE:
			cmd = new KeyboardCommand(KeyCommand::JUMP,state);
			break;
		case SDLK_ESCAPE:
			cmd = new KeyboardCommand(KeyCommand::EXIT,state);
		};
		return cmd;
	}
	else
	{
		return nullptr;
	}
}
