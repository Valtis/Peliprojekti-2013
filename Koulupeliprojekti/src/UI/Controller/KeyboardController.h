#pragma once
#include <SDL.h>
#include <map>
#include "UI/Controller/Controller.h"
#include "Message/Commands/ControlCommand.h"
#include <string>

class KeyboardController : public Controller
{
public:
	KeyboardController(int id,bool active = true);
	Command* HandleInput(SDL_Event event) override;
private:
	void LoadBinds();
	void AddBind(std::string, std::string);
	std::map<SDL_Keycode, Action> m_binds;
	int m_id;
};
