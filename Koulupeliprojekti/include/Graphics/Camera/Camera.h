#pragma once

#if (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 6))
  #define override
#endif

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
