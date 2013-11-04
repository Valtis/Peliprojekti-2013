#pragma once
#include <SDL.h>
#include "Component/AiComponent.h"

class WalkingAiComponent : public AiComponent
{
public:
	void Update(double ticksPassed) override;
	WalkingAiComponent();
private:
	bool m_direction;
	bool HandleCollisionMessage(Message *msg);
	Uint32 m_lastTick;
};
