#pragma once
#include "Entity/Entity.h"
#include "Component/AiComponent.h"

class FlyingAiComponent : public AiComponent
{
public:
	void Update(double ticksPassed);
	FlyingAiComponent(Entity* player);
private:
	MessageHandling HandleCollisionMessage(Message* msg) override;
	void Attack(double x, double y);
	void Follow(double x, double y);
	bool m_attacking, m_following, m_hit_a_wall;
	Entity* m_player;
	double m_player_x, m_player_y, m_attack_tick, m_collision_tick;
	int wiggle;
};
