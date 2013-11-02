#pragma once
#include "Component/Component.h"

class HealthComponent : public Component
{
public:
  HealthComponent();
  HealthComponent(int hitpoints, int maxHitpoints, int lives);
  ~HealthComponent();

  int GetHitpoints() { return m_hitpoints; }
  void SetHitpoints(int hp) { m_hitpoints = hp; }
  
  int GetMaxHitpoints() { return m_maxHitpoints; }
  void SetMaxHitpoints(int hp) { m_maxHitpoints = hp; }


  int GetLives() { return m_lives; }
  void SetLives(int lives) { m_lives = lives; }

  void TakeDamage();
protected:
  void OnAttatchingToEntity() override; 
private:

  int m_hitpoints;
  int m_maxHitpoints;
  int m_lives;
};
