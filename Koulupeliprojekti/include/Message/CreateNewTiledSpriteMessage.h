#include "Message/Message.h"


class CreateNewTiledSpriteMessage
{
public:
  CreateNewTiledSpriteMessage(int baseID, int newID, int width, int height) 
    : m_baseID(baseID), m_newID(newID), m_width(width), m_height(height)
  {

  }

  ~CreateNewTiledSpriteMessage() { }
  MessageType GetType() const override { return MessageType::CREATE_NEW_TILED_SPRITE; }
  int GetWidth() const { return m_width; }
  int GetHeight() const { return m_height; }
  int GetBaseID() const { return m_baseID; }
  int GetNewID() const { return m_newID; }


private:
  const int m_width;
  const int m_height;
  const int m_baseID;
  const int m_newID;
};