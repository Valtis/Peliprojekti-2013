#include <SDL.h>
#include "Message/Message.h"
#include "Message/Commands/ControlCommand.h"

ControlCommand::ControlCommand(Action cmd, bool state, int controller) : m_command(cmd),m_state(state),m_controller(controller)
{
}

MessageType ControlCommand::GetType() const
{
	return MessageType::CONTROL_COMMAND;
}

bool ControlCommand::GetState() const
{
	return m_state;
}

Action ControlCommand::GetCommand() const
{
	return m_command;
}
