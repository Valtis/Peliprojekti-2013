#pragma once

enum class MessageType : int { NONE };
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