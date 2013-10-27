#pragma once

enum class MessageType : int { NONE, LOCATION_CHANGE, VELOCITY_CHANGE, MOUSE_COMMAND, CONTROL_COMMAND, COLLISION };
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
