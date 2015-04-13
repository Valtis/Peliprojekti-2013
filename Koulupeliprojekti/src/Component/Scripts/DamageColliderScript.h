#pragma once
#include "Component/Component.h"

class DamageColliderScript : public Component
{
public:
  DamageColliderScript() { }
  ~DamageColliderScript() { }
protected:
  void OnAttatchingToEntity() override;
private:
  MessageHandling HandleCollisionMessage(Message *msg);
};