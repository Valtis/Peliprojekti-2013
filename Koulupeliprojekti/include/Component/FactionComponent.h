#pragma once
#include "Component/Component.h"

#include "Utility/FactionEnum.h"

class FactionComponent : public Component
{
public:
  FactionComponent();
  FactionComponent(Faction f);
  ~FactionComponent();

  Faction GetFaction() { return m_faction; }
  void SetFaction(Faction f) { m_faction = f;  }
protected:
  void OnAttatchingToEntity();
private:
  MessageHandling HandleQueryFactionMessage(Message *message);
  Faction m_faction;
};