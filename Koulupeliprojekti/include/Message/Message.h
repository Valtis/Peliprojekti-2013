#pragma once

enum class MessageType : int { NONE };
#include <functional>

// if handler returns true, the message may be passed to next handler; otherwise do not pass

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