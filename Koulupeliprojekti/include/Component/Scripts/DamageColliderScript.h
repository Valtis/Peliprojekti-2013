#pragma once
#include "Component/Component.h"

class DamageColliderScript : public Component
{
public:
  DamageColliderScript() { }
  ~DamageColliderScript() { }
protected:
  void OnAttatchingToEntity();
private:
  MessageHandling HandleCollisionMessage(Message *msg);
};