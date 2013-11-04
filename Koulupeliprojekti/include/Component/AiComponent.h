#pragma once
#include "Component/Component.h"

class AiComponent : public Component
{
public:
	virtual void Update(double ticksPassed) = 0;
protected:
	AiComponent();
	virtual void OnAttatchingToEntity();
	virtual bool HandleCollisionMessage(Message *msg) = 0;
};
