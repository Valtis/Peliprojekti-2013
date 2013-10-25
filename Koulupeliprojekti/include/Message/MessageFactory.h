#pragma once
class Message;
#include <memory>
namespace MessageFactory
{
  std::unique_ptr<Message> CreateLocationChangeMessage(double x, double y);
}