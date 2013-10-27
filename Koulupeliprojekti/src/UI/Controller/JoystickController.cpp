#include <SDL.h>
#include "UI/Controller/JoystickController.h"
#include "Message/Commands/ControlCommand.h"

JoystickController::JoystickController(int id,Uint8 joystick, bool active) : Controller(active),m_id(id), m_jindex(joystick)
{
	m_joystick = SDL_JoystickOpen(joystick);
	if (m_joystick == nullptr)
	{
		SetActive(false);
		return;
	}
	LoadBinds();
}

JoystickController::~JoystickController()
{
	SDL_JoystickClose(m_joystick);
}

void JoystickController::LoadBinds()
{
	// TODO: Load binds
}



Command* JoystickController::HandleInput(SDL_Event event)
{
	if (!m_active)
		return nullptr;

	Command* cmd = nullptr;

	switch(event.type)
	{
	case SDL_JOYAXISMOTION:
		if (event.jaxis.which != m_jindex)
			break;
		cmd = HandleJaxis(event);
		break;
	case SDL_JOYBALLMOTION:
		if (event.jball.which != m_jindex)
			break;
		// handle joyball
		break;
	case SDL_JOYHATMOTION:
		if (event.jball.which != m_jindex)
			break;
		// handle joyhat
		break;
	case SDL_JOYBUTTONUP:
	case SDL_JOYBUTTONDOWN:
		if (event.jbutton.which != m_jindex)
			break;
		cmd = HandleButtons(event);
	}
	return cmd;
}

Command* JoystickController::HandleJaxis(SDL_Event event)
{
	return nullptr;
}

Command* JoystickController::HandleButtons(SDL_Event event)
{
	return nullptr;
}
