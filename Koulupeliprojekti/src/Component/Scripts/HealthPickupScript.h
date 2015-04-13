#pragma once
#include "Component/Component.h"

class HealthPickupScript : public Component
{
public:
  HealthPickupScript(int health);
  ~HealthPickupScript();
protected:
  void OnAttatchingToEntity() override;
private:
  MessageHandling HandleCollisionMessage(Message *msg);
  int m_healthOnPickup;
};