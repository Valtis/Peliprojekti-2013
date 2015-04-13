#pragma once
#include "Component/Component.h"

enum class Faction : int { NONE, PLAYER, ALLIED, ENEMY };

class FactionComponent : public Component
{
public:
  FactionComponent();
  FactionComponent(Faction f);
  ~FactionComponent();

  Faction GetFaction() { return m_faction; }
  void SetFaction(Faction f) { m_faction = f;  }

private:
  Faction m_faction;
};