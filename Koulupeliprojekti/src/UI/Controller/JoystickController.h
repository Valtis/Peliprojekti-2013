#pragma once
#include <SDL.h>
#include "UI/Controller/Controller.h"
#include "Message/Commands/ControlCommand.h"

class JoystickController : public Controller
{
public:
	JoystickController(int id,Uint8 joystick,bool active = true);
	Command* HandleInput(SDL_Event event) override;
	~JoystickController() override;
private:
	JoystickController() {}
	Command* HandleJaxis(SDL_Event event);
	Command* HandleButtons(SDL_Event event);
	void LoadBinds();
	SDL_Joystick* m_joystick;
	Uint8 m_jindex;
	int m_id;
};
