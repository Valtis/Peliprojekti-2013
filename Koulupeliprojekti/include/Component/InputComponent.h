#pragma once

#include "Component/Component.h"
#include <SDL_types.h>
class Command;
class InputManager;
class InputComponent : public Component
{
public:
  InputComponent();
  InputComponent(int id);
  ~InputComponent();

  void OnAttatchingToEntity() override;
  // possibly redo
  void RegisterInputHandler(InputManager &manager);

private:

  MessageHandling HandleInput(Command *msg);
  void Fire();
  void Jump();
  Uint32 m_lastFireTick;
  int m_id;

};
