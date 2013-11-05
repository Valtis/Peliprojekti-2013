#pragma once
#include "Entity/Entity.h"
#include "Component/AiComponent.h"

class FlyingAiComponent : public AiComponent
{
public:
	void Update(double ticksPassed);
	FlyingAiComponent(Entity* player);
private:
	bool HandleCollisionMessage(Message* msg);
	void Attack(double x, double y);
	void Follow(double x, double y);
	void Move(double ticksPassed);
	bool m_attacking;
	bool y_direction;
	Entity* m_player;
	double m_player_x, m_player_y;

	double m_last_tick;
};
