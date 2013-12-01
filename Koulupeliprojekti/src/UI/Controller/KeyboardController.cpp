#include <SDL.h>
#include "UI/Controller/KeyboardController.h"
#include "Message/Commands/ControlCommand.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

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
		KeyState state = KeyState::UP; 
	if (event.type == SDL_KEYDOWN)
	{
		state = KeyState::DOWN;
	}
    
		SDL_Keycode key = event.key.keysym.sym;
		if (m_binds.count(key))
			cmd = new ControlCommand(m_binds[key], state,m_id);
	}
	return cmd;
}


void KeyboardController::LoadBinds()
{
	std::ifstream file;
	file.open("data/binds/binds.dat");
	bool loaded = false;
	if (file.is_open())
	{
		std::string line = "";
		while (line != "[KEYBOARD]") std::getline(file,line);
		while (!file.eof())
		{
			std::getline(file,line);
			if (line == "")
				break;
			std::stringstream ssin(line);
			std::string keycode, action;
			ssin >> action;
			ssin >> keycode;
			ssin >> keycode;
			std::transform(keycode.begin(),keycode.end(),keycode.begin(),::tolower);
			std::transform(action.begin(),action.end(),action.begin(),::tolower);
			AddBind(keycode, action);
			loaded = true;
		}
		file.close();
	}
	if (loaded)
		return;
	m_binds[SDLK_LEFT] = Action::LEFT;
	m_binds[SDLK_RIGHT] = Action::RIGHT;
	m_binds[SDLK_SPACE] = Action::JUMP;
 	m_binds[SDLK_LSHIFT] = Action::FIRE;
	m_binds[SDLK_ESCAPE] = Action::EXIT;
 	m_binds[SDLK_p] = Action::PAUSE;
 	m_binds[SDLK_PAUSE] = Action::PAUSE;
}

void KeyboardController::AddBind(std::string keycode, std::string actionstring)
{
	if (keycode.length() < 1 || actionstring.length() < 1)
		return;
	SDL_Keycode code;
	Action action;
	if (keycode =="left")
		code = SDLK_LEFT;
	else if (keycode =="right")
		code = SDLK_RIGHT;
	else if (keycode =="up")
		code = SDLK_UP;
	else if (keycode =="down")
		code = SDLK_DOWN;
	else if (keycode =="lshift")
		code = SDLK_LSHIFT;
	else if (keycode =="rshift")
		code = SDLK_RSHIFT;
	else if (keycode == "space")
		code = SDLK_SPACE;
	else if (keycode == "esc")
		code = SDLK_ESCAPE;
	else if (keycode.length() == 1)
		code = static_cast<int>(keycode.c_str()[0]);
	else
		return;

	if (actionstring == "shoot")
		action = Action::FIRE;
	else if (actionstring == "jump")
		action = Action::JUMP;
	else if (actionstring == "left")
		action = Action::LEFT;
	else if (actionstring == "right")
		action = Action::RIGHT;
	else if (actionstring == "up")
		action = Action::UP;
	else if (actionstring == "down")
		action = Action::DOWN;
	else if (actionstring == "exit")
		action = Action::EXIT;
	else if (actionstring == "pause")
		action = Action::PAUSE;
	else
		return;
	m_binds[code] = action;
}
