#pragma once

class Camera
{
public:
  virtual ~Camera() { }

  virtual int GetX() = 0;
  virtual int GetY() = 0;

protected:
  Camera() { }

private:

};
