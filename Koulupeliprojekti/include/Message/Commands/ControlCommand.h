#pragma once
#include <SDL.h>
#include "Message/Commands/Command.h"
#include "Message/Message.h"
enum class Action : int { NONE, LEFT, RIGHT, UP, DOWN, JUMP, FIRE, EXIT, PAUSE };
enum class KeyState : int { DOWN, UP };

class ControlCommand : public Command
{
public:
	ControlCommand(Action cmd, KeyState state, int controller);
	MessageType GetType() const;
	KeyState GetState() const;
	Action GetCommand() const;
	int GetController() const { return m_controller; }
private:
	KeyState m_state;
	Action m_command;
	int m_controller;
};
