#pragma once

enum class MessageType : int { NONE, LOCATION_CHANGE, VELOCITY_CHANGE, SET_VELOCITY,  MOUSE_COMMAND, CONTROL_COMMAND, COLLISION, SPAWN_ENTITY, TERMINATE_ENTITY };
#include <functional>


class Message
{
public:
  virtual ~Message();
  virtual MessageType GetType() const = 0;
protected:
  Message();
private:

};


typedef std::function<bool(Message *)> MessageCallback;
enum class Priority : int { LOWEST, LOW, NORMAL, HIGH, HIGHEST };
