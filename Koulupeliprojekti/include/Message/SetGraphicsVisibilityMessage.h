#include "Message/Message.h"

class SetGraphicsVisibilityMessage : public Message
{
public:
  SetGraphicsVisibilityMessage(bool visibility) : m_visibility(visibility) { }
  ~SetGraphicsVisibilityMessage() { }

  MessageType GetType() const override { return MessageType::SET_GRAPHICS_VISIBILITY; }
  bool GetVisibility() const { return m_visibility; }
private:
  const bool m_visibility;
};