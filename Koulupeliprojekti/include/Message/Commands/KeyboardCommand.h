#pragma once
#include <SDL.h>
#include "Message/Commands/Command.h"
#include "Message/Message.h"

class KeyboardCommand : public Command
{
public:
	KeyboardCommand(SDL_Event event) : m_state(event.key.type), m_sym(event.key.keysym) {}
	MessageType GetType() const { return MessageType::KEYBOARD_COMMAND; }
	Uint8 GetState() { return m_state; }
	SDL_Keysym GetKeySym() { return m_sym; }
private:
	Uint8 m_state;
	SDL_Keysym m_sym;
};
