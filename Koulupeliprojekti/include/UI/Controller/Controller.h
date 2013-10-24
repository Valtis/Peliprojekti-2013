#pragma once
#include <SDL.h>
#include "Message/Commands/Command.h"

class Controller
{
public:
	Controller() : m_active(true) {}
	Controller(bool active) : m_active(active) {}
	virtual ~Controller() {}
	virtual Command* HandleInput(SDL_Event event) = 0;
	virtual void SetActive(bool active) { m_active = active; }
protected:
	bool m_active;
};
