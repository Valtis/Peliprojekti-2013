#pragma once

#if (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 6))
  #define override
#endif

enum class MessageHandling : int { PASS_FORWARD, STOP_HANDLING };
enum class MessageType : int { NONE, LOCATION_CHANGE, SET_LOCATION, VELOCITY_CHANGE, 
  SET_VELOCITY,  MOUSE_COMMAND, CONTROL_COMMAND, COLLISION, SPAWN_ENTITY, 
  TERMINATE_ENTITY, TAKE_DAMAGE, END_LEVEL, END_GAME, CREATE_NEW_TILED_SPRITE,
  START_BLINKING, SET_GRAPHICS_VISIBILITY, RESET_ENTITY_POSITION, PLAY_SOUND_EFFECT,
  ADD_HEALTH, IMPORTANT_CHARACTER_DEATH, FIRE_DIRECTION, CHANGE_ANIMATION_MESSAGE };

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
