#include <SDL.h>
#include "Component/WalkingAiComponent.h"
#include "Entity/Entity.h"
#include "Message/CollisionMessage.h"
#include "Message/Commands/ControlCommand.h"

WalkingAiComponent::WalkingAiComponent() : m_direction(false), m_lastTick(SDL_GetTicks()), m_commandSent(false)
{

}

void WalkingAiComponent::Update(double ticksPassed)
{
	if (m_commandSent)
		return;
	if (m_direction)
		GetOwner()->SendMessage(new ControlCommand(Action::RIGHT,true,-1));
	else
		GetOwner()->SendMessage(new ControlCommand(Action::LEFT,true,-1));
	m_commandSent = true;
}

bool WalkingAiComponent::HandleCollisionMessage(Message *msg)
{
	if (msg->GetType() != MessageType::COLLISION)
		return false;
	Uint32 ticks = SDL_GetTicks();
	if (ticks >= m_lastTick + 100)
	{
		if (m_direction)
			GetOwner()->SendMessage(new ControlCommand(Action::RIGHT,false,-1));
		else
			GetOwner()->SendMessage(new ControlCommand(Action::LEFT,false,-1));
		m_direction = !m_direction;
		m_lastTick = ticks;
		m_commandSent = false;
	}
	return false;
}
