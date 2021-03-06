#include <cmath>
#include "Entity/Entity.h"
#include "Component/FollowingAiComponent.h"
#include "Message/Commands/ControlCommand.h"
#include "Component/LocationComponent.h"
#include "Component/FactionComponent.h"
#include "Message/CollisionMessage.h"

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
		if (player_x - owner_x > 10)
		{
			GetOwner()->SendMessage(new ControlCommand(Action::RIGHT,KeyState::DOWN,-1));
		}
		else if (player_x - owner_x < -10)
		{
			GetOwner()->SendMessage(new ControlCommand(Action::LEFT,KeyState::DOWN,-1));
		}
	}
}

MessageHandling FollowingAiComponent::HandleCollisionMessage(Message *msg)
{
	if (msg->GetType() != MessageType::COLLISION)
	{
		return MessageHandling::STOP_HANDLING;
	}
	CollisionMessage *collision = static_cast<CollisionMessage*>(msg);

  for (auto entity : collision->GetEntities())
  {
	  Component *faction = entity->GetComponent(ComponentType::FACTION);
	  if (faction == nullptr)
		{
      GetOwner()->SendMessage(new ControlCommand(Action::JUMP, KeyState::DOWN, -1));
      break;
    }
  }
	return MessageHandling::STOP_HANDLING;
}
