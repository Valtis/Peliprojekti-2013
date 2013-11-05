#include <cmath>
#include "Component/LocationComponent.h"
#include "Entity/Entity.h"
#include "Message/Commands/ControlCommand.h"
#include "Component/FlyingAiComponent.h"

FlyingAiComponent::FlyingAiComponent(Entity* player) : AiComponent(), m_player(player), m_attacking(false), m_last_tick(0), y_direction(true)
{

}

void FlyingAiComponent::Update(double ticksPassed)
{
	LocationComponent* player_loc = static_cast<LocationComponent*>(m_player->GetComponent(ComponentType::LOCATION));
	LocationComponent* own_loc = static_cast<LocationComponent*>(GetOwner()->GetComponent(ComponentType::LOCATION));

	if(player_loc == nullptr || own_loc == nullptr)
		return;

	double player_x = player_loc->GetX();
	double player_y = player_loc->GetY();

	double x = own_loc->GetX();
	double y = own_loc->GetY();

	double distance = sqrt(pow(player_x - x,2) + pow(player_y - y,2));
	

	if (!m_attacking)
	{
		m_player_x = player_x;
		m_player_y = player_y;
		Follow(x,y);
		Move(ticksPassed);
	}
	else
	{
		Attack(x,y);
	}
}

void FlyingAiComponent::Move(double ticksPassed)
{
	if (ticksPassed - m_last_tick < 0.5)
		return;
	m_last_tick = ticksPassed;
	if (y_direction)
	{
		GetOwner()->SendMessage(new ControlCommand(Action::DOWN,false,-1)); 
		GetOwner()->SendMessage(new ControlCommand(Action::UP,true,-1)); 
	}
	else
	{
		GetOwner()->SendMessage(new ControlCommand(Action::UP,false,-1)); 
		GetOwner()->SendMessage(new ControlCommand(Action::DOWN,true,-1)); 
	}
}

bool FlyingAiComponent::HandleCollisionMessage(Message *msg)
{
	return true;
}

void FlyingAiComponent::Attack(double x, double y)
{
	m_attacking = false;
}

void FlyingAiComponent::Follow(double x, double y)
{
	if (x > m_player_x + 50)
	{
		GetOwner()->SendMessage(new ControlCommand(Action::RIGHT,false,-1));
		GetOwner()->SendMessage(new ControlCommand(Action::LEFT,true,-1));
	}
	else if (x < m_player_x - 50)
	{
		GetOwner()->SendMessage(new ControlCommand(Action::LEFT,false,-1));
		GetOwner()->SendMessage(new ControlCommand(Action::RIGHT,true,-1));
	}
	if (m_player_y - y < 200)
	{
		GetOwner()->SendMessage(new ControlCommand(Action::DOWN,false,-1));
		GetOwner()->SendMessage(new ControlCommand(Action::UP,true,-1));
	}
	else if (m_player_y - y > 300)
	{
		GetOwner()->SendMessage(new ControlCommand(Action::UP,false,-1));
		GetOwner()->SendMessage(new ControlCommand(Action::DOWN,true,-1));
	}
}
