#pragma once

class ButtonHandler 
{
public:
  virtual ~ButtonHandler();

  virtual void OnButtonClick() = 0;

protected:
  ButtonHandler() { }
};