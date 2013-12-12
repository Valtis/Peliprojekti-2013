#include <cmath>
#include "Component/FollowingAiComponent.h"
#include "Message/MessageFactory.h"
#include "Message/QueryLocationMessage.h"
#include "Message/QueryFactionMessage.h"
#include "Message/Commands/ControlCommand.h"
#include "Message/CollisionMessage.h"
#include "Entity/Entity.h"
FollowingAiComponent::FollowingAiComponent(Entity *player) : m_player(player)
{
}

void FollowingAiComponent::Update(double ticksPassed)
{
  auto playerLocationQuery = MessageFactory::CreateQueryLocationMessage();
  auto ownLocationQuery =  MessageFactory::CreateQueryLocationMessage();

  if (!GetOwner()->SendMessage(ownLocationQuery.get()) || !m_player->SendMessage(playerLocationQuery.get()))
  {
    return;
  }


	double player_x = playerLocationQuery->GetX();
	double owner_x = ownLocationQuery->GetX();
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
    auto queryFactionMessage = MessageFactory::CreateQueryFactionMessage();
   
    if (!entity->SendMessage(queryFactionMessage.get()))
		{
      GetOwner()->SendMessage(new ControlCommand(Action::JUMP, KeyState::DOWN, -1));
      break;
    }
  }
	return MessageHandling::STOP_HANDLING;
}
