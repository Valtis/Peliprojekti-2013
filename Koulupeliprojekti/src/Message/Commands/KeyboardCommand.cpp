#include <SDL.h>
#include "Message/Message.h"
#include "Message/Commands/KeyboardCommand.h"

KeyboardCommand::KeyboardCommand(KeyCommand cmd, bool state) : m_command(cmd),m_state(state)
{
}

MessageType KeyboardCommand::GetType() const
{
	return MessageType::KEYBOARD_COMMAND;
}

bool KeyboardCommand::GetState() const
{
	return m_state;
}

KeyCommand KeyboardCommand::GetCommand() const
{
	return m_command;
}
