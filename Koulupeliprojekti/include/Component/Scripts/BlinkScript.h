#include "Component/Component.h"

class BlinkScript : public Component
{
public:
  BlinkScript(double singleBlinkDuration) : m_blinkDuration(0), 
    m_singleBlinkDuration(singleBlinkDuration) { }
  ~BlinkScript() { }
  void Update(double ticks) override;
protected:
  void OnAttatchingToEntity() override;
private:
  double m_blinkDuration; // how long it keeps blinking
  double m_singleBlinkDuration; // how long single frame is shown\hidden

};