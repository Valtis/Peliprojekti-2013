#include <SDL.h>
#include <memory>
#include "Component/WalkingAiComponent.h"
#include "Entity/Entity.h"
#include "Message/CollisionMessage.h"
#include "Message/Commands/ControlCommand.h"

WalkingAiComponent::WalkingAiComponent() : m_direction(true), m_lastTick(SDL_GetTicks())
{

}

void WalkingAiComponent::Update(double ticksPassed)
{
	if (GetOwner() == nullptr)
		return;
	std::unique_ptr<ControlCommand> command;
	if (m_direction)
	{
		command.reset(new ControlCommand(Action::RIGHT,KeyState::DOWN,-1));
		GetOwner()->SendMessage(command.get());
	}
	else
	{
		command.reset(new ControlCommand(Action::LEFT,KeyState::DOWN,-1));
		GetOwner()->SendMessage(command.get());
	}
}

MessageHandling WalkingAiComponent::HandleCollisionMessage(Message *msg)
{
	if (msg->GetType() != MessageType::COLLISION)
		return MessageHandling::STOP_HANDLING;
	std::unique_ptr<ControlCommand> command;

	Uint32 ticks = SDL_GetTicks();
	if (ticks >= m_lastTick + 100)
	{
		if (m_direction)
		{
			command.reset(new ControlCommand(Action::RIGHT,KeyState::UP,-1));
			GetOwner()->SendMessage(command.get());
		}
		else
		{
			command.reset(new ControlCommand(Action::RIGHT,KeyState::UP,-1));
			GetOwner()->SendMessage(command.get());
		}
		m_direction = !m_direction;
		m_lastTick = ticks;
	}
	return MessageHandling::STOP_HANDLING;
}
