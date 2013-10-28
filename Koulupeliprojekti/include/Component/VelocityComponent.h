#pragma once
#include "Component/Component.h"

class VelocityComponent : public Component
{
public:
  VelocityComponent();
  ~VelocityComponent();
  void OnAttatchingToEntity() override;
  void Update(double ticksPassed) override;

private:
  bool HandleVelocityChangeMessage(Message *msg);
  bool HandleSetVelocityMessage(Message *msg);

  double m_xVelocity;
  double m_yVelocity;
};