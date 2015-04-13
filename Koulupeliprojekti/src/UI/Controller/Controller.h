#pragma once
#include <SDL.h>
#include "Message/Commands/Command.h"

class Controller
{
public:
	
	Controller(bool active) : m_active(active) {}
	virtual ~Controller() {}
	virtual Command* HandleInput(SDL_Event event) = 0;
	virtual void SetActive(bool active) { m_active = active; }
protected:
  Controller() : m_active(true) {}
	bool m_active;
};
