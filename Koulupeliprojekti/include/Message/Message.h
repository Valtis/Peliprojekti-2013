#pragma once
enum class MessageHandling : int { PASS_FORWARD, STOP_HANDLING };
enum class MessageType : int { NONE, LOCATION_CHANGE, VELOCITY_CHANGE, SET_VELOCITY,  MOUSE_COMMAND, 
  CONTROL_COMMAND, COLLISION, SPAWN_ENTITY, TERMINATE_ENTITY, TAKE_DAMAGE, END_LEVEL, END_GAME };
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


typedef std::function<MessageHandling(Message *)> MessageCallback;
enum class Priority : int { LOWEST, LOW, NORMAL, HIGH, HIGHEST };
