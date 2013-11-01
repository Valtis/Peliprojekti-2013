#pragma once

#include "Component/Component.h"
#include <SDL_types.h>
class Command;
class InputManager;
class InputComponent : public Component
{
public:
  InputComponent();
  ~InputComponent();

  void OnAttatchingToEntity() override;
  // possibly redo
  void RegisterInputHandler(InputManager &manager);

private:
  bool HandleInput(Command *msg);
  void Fire();
  Uint32 m_debugLastFireTick;

};