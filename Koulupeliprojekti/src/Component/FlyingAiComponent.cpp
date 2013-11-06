#include <SDL.h>
#include <cmath>
#include "Component/LocationComponent.h"
#include "Entity/Entity.h"
#include "Message/CollisionMessage.h"
#include "Message/Commands/ControlCommand.h"
#include "Component/FactionComponent.h"
#include "Component/FlyingAiComponent.h"

FlyingAiComponent::FlyingAiComponent(Entity* player) : AiComponent(), m_player(player), m_attacking(false), m_attack_tick(0), m_following(true)
{

}

void FlyingAiComponent::Update(double ticksPassed)
{
	m_attack_tick += ticksPassed;
	LocationComponent* player_loc = static_cast<LocationComponent*>(m_player->GetComponent(ComponentType::LOCATION));
	LocationComponent* own_loc = static_cast<LocationComponent*>(GetOwner()->GetComponent(ComponentType::LOCATION));

	if(player_loc == nullptr || own_loc == nullptr)
		return;

	double player_x = player_loc->GetX();
	double player_y = player_loc->GetY();

	double x = own_loc->GetX();
	double y = own_loc->GetY();

	double distance = sqrt(pow(player_x - x,2) + pow(player_y - y,2));
	if (abs(m_player_y - y) > 230)
		m_following = true;
	

	if (m_following)
	{
		m_player_x = player_x;
		m_player_y = player_y;
		Follow(x,y);
		if (abs(player_x - x) < 200 && player_y - y < 230)
		{
			if (m_attack_tick > 150)
			{
				m_attack_tick = 0;
				m_attacking = true;
				m_following = false;
				wiggle = 50;
			}
		}
	}
	else
	{
		if (m_attacking)
		{
			Attack(x,y);
			wiggle -= static_cast<int>(ticksPassed / 100.0);
		}
	}
}

bool FlyingAiComponent::HandleCollisionMessage(Message *msg)
{

	if (msg->GetType() != MessageType::COLLISION)
		return false;

	m_attacking = false;
	GetOwner()->SendMessage(new ControlCommand(Action::UP,true,-1));

	return true;
}

void FlyingAiComponent::Attack(double x, double y)
{
	if (abs(m_player_x - x) < 20 && abs(m_player_y - y) < 20)
	{
	GetOwner()->SendMessage(new ControlCommand(Action::UP,true,-1));
	m_attacking = false;
	return;
	}

	if (x > m_player_x + wiggle)
	{
		GetOwner()->SendMessage(new ControlCommand(Action::LEFT,true,-1));
	}
	else if (x < m_player_x - wiggle)
	{
		GetOwner()->SendMessage(new ControlCommand(Action::RIGHT,true,-1));
	}
		
	GetOwner()->SendMessage(new ControlCommand(Action::DOWN,true,-1));
		
}

void FlyingAiComponent::Follow(double x, double y)
{
	if (x > m_player_x + 200)
	{
		GetOwner()->SendMessage(new ControlCommand(Action::LEFT,true,-1));
	}
	else if (x < m_player_x - 50)
	{
		GetOwner()->SendMessage(new ControlCommand(Action::RIGHT,true,-1));
	}
	if (m_player_y - y < 200)
	{
		GetOwner()->SendMessage(new ControlCommand(Action::UP,true,-1));
	}
	else if (m_player_y - y > 320)
	{
		GetOwner()->SendMessage(new ControlCommand(Action::DOWN,true,-1));
	}
}
