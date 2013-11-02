#pragma once
#include "Component/Component.h"

class BulletScriptComponent : public Component
{
public:
  BulletScriptComponent();
  ~BulletScriptComponent();

protected:
  void OnAttatchingToEntity() override;
private:
  bool HandleCollisionMessage(Message *msg);
};