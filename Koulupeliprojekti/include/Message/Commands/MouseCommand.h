#pragma once
#include <SDL.h>
#include "Message/Command/Command.h"

class MouseCommand : public Command
{
public:
	MouseCommand(const SDL_Event& event) : m_event(event) {}
	MessageType GetType() const {return NONE;}
	const SDL_Event GetEvent() { return event; }
private:
	const SDL_Event event;
};
