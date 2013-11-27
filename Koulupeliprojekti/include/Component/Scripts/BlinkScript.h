#include "Component/Component.h"

class BlinkScript : public Component
{
public:
  BlinkScript(double singleBlinkDuration) : m_blinkDuration(0), 
    m_ticksUntilFlip(0), m_lastState(false),
    m_singleBlinkDuration(singleBlinkDuration) { }
  ~BlinkScript() { }
  void Update(double ticks) override;

  void FlipGraphicsState();



protected:
  void OnAttatchingToEntity() override;
private:
  void StopBlinking();
  MessageHandling HandleStartBlinkingMessage(Message *message);
  double m_blinkDuration; // how long it keeps blinking
  double m_ticksUntilFlip;
  bool m_lastState;
  const double m_singleBlinkDuration; // how long single frame is shown\hidden

};