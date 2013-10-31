#include <cmath>
#include "Entity/Entity.h"
#include "Component/FollowingAiComponent.h"
#include "Message/Commands/ControlCommand.h"
#include "Component/LocationComponent.h"

FollowingAiComponent::FollowingAiComponent(Entity *player) : m_player(player)
{
}

void FollowingAiComponent::Update(double ticksPassed)
{
	LocationComponent* player_loc = static_cast<LocationComponent*>(m_player->GetComponent(ComponentType::LOCATION));
	LocationComponent* owner_loc = static_cast<LocationComponent*>(GetOwner()->GetComponent(ComponentType::LOCATION));
	double player_x = player_loc->GetX();
	double owner_x = owner_loc->GetX();
	if (abs(player_x - owner_x) < 500)
	{
		if (player_x > owner_x)
		{
			GetOwner()->SendMessage(new ControlCommand(Action::RIGHT,true,-1));
		}
		else
		{
			GetOwner()->SendMessage(new ControlCommand(Action::LEFT,true,-1));
		}
	}
}

bool FollowingAiComponent::HandleCollisionMessage(Message *msg)
{
	if (msg->GetType() != MessageType::COLLISION)
	{
		return false;
	}
	// Check direction, if hit a wall, jump
	return false;
}
