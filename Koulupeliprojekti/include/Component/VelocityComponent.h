#pragma once
#include "Component/Component.h"

class VelocityComponent : public Component
{
public:
  VelocityComponent();
  ~VelocityComponent();
  void OnAttatchingToEntity() override;
  void Update(double ticksPassed) override;
  void SetVelocity(double x, double y)
  {
    m_xVelocity = x;
    m_yVelocity = y;
  }
private:
  bool HandleVelocityChangeMessage(Message *msg);
  bool HandleSetVelocityMessage(Message *msg);

  double m_xVelocity;
  double m_yVelocity;
};