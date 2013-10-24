#pragma once
#include <SDL.h>
#include <queue>
#include <map>
#include "UI/Controller/Controller.h"
#include "Message/Commands/KeyboardCommand.h"

class KeyboardController : public Controller
{
public:
	KeyboardController(bool active = true);
	Command* HandleInput(SDL_Event event) override;
private:
	void LoadBinds();
	std::map<SDL_Keycode, Action> m_binds;
};
