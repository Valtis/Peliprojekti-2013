#include <SDL.h>
#include "UI/Controller/KeyboardController.h"
#include "Message/Commands/ControlCommand.h"

KeyboardController::KeyboardController(int id,bool active) : Controller(active), m_id(id)
{
	LoadBinds();
}
Command* KeyboardController::HandleInput(SDL_Event event)
{
	if (!m_active)
		return nullptr;
	Command* cmd = nullptr;
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	{
		bool state = (event.type == SDL_KEYDOWN);
		SDL_Keycode key = event.key.keysym.sym;
		if (m_binds.count(key))
			cmd = new ControlCommand(m_binds[key],state,m_id);
	}
	return cmd;
}
void KeyboardController::LoadBinds()
{
	m_binds[SDLK_DOWN] = Action::DOWN;
	m_binds[SDLK_UP] = Action::UP;
	m_binds[SDLK_LEFT] = Action::LEFT;
	m_binds[SDLK_RIGHT] = Action::RIGHT;
	m_binds[SDLK_SPACE] = Action::JUMP;
	m_binds[SDLK_ESCAPE] = Action::EXIT;
}
