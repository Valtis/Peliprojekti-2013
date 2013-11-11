#pragma once
#include "Component/AiComponent.h"

class FollowingAiComponent : public AiComponent
{
public:
	FollowingAiComponent(Entity *player);
	void Update(double ticksPassed) override;
private:
	MessageHandling HandleCollisionMessage(Message *msg) override;
	Entity* m_player;
};
