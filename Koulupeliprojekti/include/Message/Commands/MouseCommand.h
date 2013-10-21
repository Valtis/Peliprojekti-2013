#pragma once
#include <SDL.h>
#include "Message/Commands/Command.h"

class MouseCommand : public Command
{
public:
	MouseCommand(const SDL_Event& event) : m_event(event) {}
	MessageType GetType() const {return MessageType::MOUSE_COMMAND; }
	const SDL_Event GetEvent() { return m_event; }
private:
	const SDL_Event m_event;
};
