#include "UI/Window/Window.h"
#include "Graphics/TextureFactory.h"
Window::Window() 
{
  // create common window features (title bar - close button)
}
// test code - move sprite construction to proper class/method
Window::Window(SDL_Rect location, SDL_Color color, Renderer *renderer)
{
  SetRect(location);
  SetTexture(TextureFactory::CreateWindowTexture(location.w, location.h, color, renderer));
}

Window::~Window()
{

}

