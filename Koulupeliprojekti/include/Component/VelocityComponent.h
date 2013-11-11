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
  void AddVelocity(double x, double y)
  {
    m_xVelocity += x;
    m_yVelocity += y;
  }
  double GetVelocityX()
  {
    return m_xVelocity;
  }
  double GetVelocityY()
  {
    return m_yVelocity;
  }
private:
  MessageHandling HandleVelocityChangeMessage(Message *msg);
  MessageHandling HandleSetVelocityMessage(Message *msg);

  double m_xVelocity;
  double m_yVelocity;
};