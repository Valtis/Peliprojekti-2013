#pragma once
#include "Component/Component.h"

class VelocityComponent : public Component
{
public:
  VelocityComponent(double xVelocity, double yVelocity);
  ~VelocityComponent();
  void OnAttatchingToEntity() override;
  void Update(double ticksPassed) override;
  void SetVelocity(double x, double y)
  {
    m_currentXVelocity = x;
    m_currentYVelocity = y;
  }
  void AddVelocity(double x, double y)
  {
    m_currentXVelocity += x;
    m_currentYVelocity += y;
  }
  double GetVelocityX()
  {
    return m_currentXVelocity;
  }
  double GetVelocityY()
  {
    return m_currentYVelocity;
  }
private:

  MessageHandling HandleSetVelocityMessage(Message *msg);

  double m_currentXVelocity;
  double m_currentYVelocity;

  const double m_xChangeVelocity;
  const double m_yChangeVelocity;

};