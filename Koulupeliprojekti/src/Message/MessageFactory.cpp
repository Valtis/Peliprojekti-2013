#include "Message/MessageFactory.h"
#include "Message/LocationChangeMessage.h"

std::unique_ptr<Message> MessageFactory::CreateLocationChangeMessage(double x, double y)
{
  return std::unique_ptr<Message>(new LocationChangeMessage(x, y));
}
