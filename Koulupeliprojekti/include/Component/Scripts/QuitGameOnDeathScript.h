#pragma once
#include "Component/Component.h"

class QuitGameOnDeathScript : public Component
{
public:
  QuitGameOnDeathScript() { }
  ~QuitGameOnDeathScript() { }
protected:
  void OnAttatchingToEntity() override;
private:
  MessageHandling HandleTerminationMessage(Message *msg);
};