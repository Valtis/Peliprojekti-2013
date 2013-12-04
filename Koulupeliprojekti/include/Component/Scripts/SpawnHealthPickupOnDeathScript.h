#pragma once
#include "Component/Component.h"
#include <random>

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
  std::mt19937 m_randomEngine;
};