#pragma once
#include "Component/Component.h"

class BulletScriptComponent : public Component
{
public:
  BulletScriptComponent();
  ~BulletScriptComponent();

  void Update(double ticksPassed) override;

protected:
  void OnAttatchingToEntity() override;
private:
  double m_lifeTimeRemaining;

  MessageHandling HandleCollisionMessage(Message *msg);
};