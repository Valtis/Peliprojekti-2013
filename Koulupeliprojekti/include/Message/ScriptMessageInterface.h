#pragma once

class Entity;

class ScriptMessageInterface {
public:
  void SendBlinkingMessage(Entity *entity, int duration);
};