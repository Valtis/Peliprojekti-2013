#pragma once
#include "Component/Component.h"

class SpawnHealthPickupOnDeathScript : public Component
{
public:
  SpawnHealthPickupOnDeathScript(int dropChange);
  ~SpawnHealthPickupOnDeathScript();
protected:
  void OnAttatchingToEntity();
private:
  MessageHandling HandleTerminateMessage(Message *msg);
  int m_dropChange;
};