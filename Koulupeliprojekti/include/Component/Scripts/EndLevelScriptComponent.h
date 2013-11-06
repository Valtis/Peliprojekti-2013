#pragma once
#include "Component/Component.h"

class EndLevelScriptComponent : public Component
{
public:
	EndLevelScriptComponent();
private:
	void OnAttatchingToEntity() override;
	bool HandleCollisionMessage(Message* msg);
};
