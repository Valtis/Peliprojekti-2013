#pragma once
#include <SDL.h>
#include "Message/Commands/Command.h"
#include "Message/Message.h"
enum class Action : int { NONE, LEFT, RIGHT, UP, DOWN, JUMP, EXIT };

class ControlCommand : public Command
{
public:
	ControlCommand(Action cmd, bool state, int controller);
	MessageType GetType() const;
	bool GetState() const;
	Action GetCommand() const;
	int GetController() const { return m_controller; }
private:
	bool m_state;
	Action m_command;
	int m_controller;
};
