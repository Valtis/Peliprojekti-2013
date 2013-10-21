#pragma once
#include <SDL.h>
#include "Message/Commands/Command.h"
#include "Message/Message.h"
enum class Action : int { NONE, LEFT, RIGHT, UP, DOWN, JUMP, EXIT };

class KeyboardCommand : public Command
{
public:
	KeyboardCommand(Action cmd, bool state);
	MessageType GetType() const;
	bool GetState() const;
	Action GetCommand() const;
private:
	bool m_state;
	Action m_command;
};
