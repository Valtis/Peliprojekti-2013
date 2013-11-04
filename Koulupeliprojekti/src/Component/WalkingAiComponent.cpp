#include <SDL.h>
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
	if (m_direction)
		GetOwner()->SendMessage(new ControlCommand(Action::RIGHT,true,-1));
	else
		GetOwner()->SendMessage(new ControlCommand(Action::LEFT,true,-1));
}

bool WalkingAiComponent::HandleCollisionMessage(Message *msg)
{
	if (msg->GetType() != MessageType::COLLISION)
		return true;
	Uint32 ticks = SDL_GetTicks();
	if (ticks >= m_lastTick + 100)
	{
		if (m_direction)
			GetOwner()->SendMessage(new ControlCommand(Action::RIGHT,false,-1));
		else
			GetOwner()->SendMessage(new ControlCommand(Action::LEFT,false,-1));
		m_direction = !m_direction;
		m_lastTick = ticks;
	}
	return false;
}
