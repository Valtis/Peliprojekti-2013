#pragma once

#include "Component/Component.h"
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
};