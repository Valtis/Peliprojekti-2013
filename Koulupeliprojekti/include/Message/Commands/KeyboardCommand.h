#pragma once
#include <SDL.h>
#include "Message/Commands/Command.h"
#include "Message/Message.h"
enum class KeyCommand : int { NONE, LEFT, RIGHT, UP, DOWN, JUMP, EXIT };

class KeyboardCommand : public Command
{
public:
	KeyboardCommand(KeyCommand cmd, bool state);
	MessageType GetType() const;
	bool GetState() const;
	KeyCommand GetCommand() const;
private:
	bool m_state;
	KeyCommand m_command;
};
