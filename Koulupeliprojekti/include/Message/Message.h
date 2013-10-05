#pragma once

enum class MessageType : int { NONE };
#include <functional>
typedef std::function<void(Message *)> MessageCallback;


class Message
{
public:
  virtual ~Message();
  MessageType GetType() = 0;
protected:
  Message();
private:

};