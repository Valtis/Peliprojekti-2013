#include "Graphics/FontManager.h"
#include "Graphics/Renderer.h"
FontManager::FontManager()
{

}

FontManager::~FontManager()
{
  for (auto &font : m_fonts)
  {
    TTF_CloseFont(font.second);
  }
}

FontManager &FontManager::Instance()
{
  static FontManager m_instance;
  return m_instance;
}

double FontManager::GetFontWidth(int fontSize)
{
  CalculateFontSize(fontSize);
  return m_fontSizes[fontSize].first;
}

double FontManager::GetFontHeight(int fontSize)
{
  CalculateFontSize(fontSize);
  return m_fontSizes[fontSize].second;
}

void FontManager::Initialize(std::string pathAndName)
{
  m_fontFilePathAndName = pathAndName;
}


SDL_Texture *FontManager::GetTextSingleLine(Renderer *renderer, std::string text, int fontSize)
{
  SDL_Color color = { 0, 0, 0, 255 };
  return GetTextSingleLine(renderer, text, fontSize, color);
}

SDL_Texture *FontManager::GetTextSingleLine(Renderer *renderer, std::string text, int fontSize, SDL_Color color)
{
  OpenFontFile(fontSize);

  SDL_Surface *textSurface = TTF_RenderText_Blended(m_fonts[fontSize], text.c_str(), color);
  SDL_assert_release(textSurface != nullptr);
  
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer->GetRenderingContext(), textSurface);
  SDL_FreeSurface(textSurface);
  SDL_assert_release(texture != nullptr);
  return texture;
}

void FontManager::OpenFontFile(int fontSize)
{
  if (m_fonts.count(fontSize) == 0)
  {
    m_fonts[fontSize] =  TTF_OpenFont(m_fontFilePathAndName.c_str(), fontSize);
    SDL_assert_release(m_fonts[fontSize] != nullptr);
  }
}

void FontManager::CalculateFontSize(int size)
{
  OpenFontFile(size);

  if (m_fontSizes.count(size) != 0)
  {
    return;
  }

  std::string text = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  SDL_Color text_color = {0, 0, 0, 255};

  SDL_Surface *textSurface = TTF_RenderText_Blended(m_fonts[size], text.c_str(), text_color);
  SDL_assert_release(textSurface != nullptr);
  
  double width = textSurface->w;
  double height = textSurface->h;
  double length = text.length();
  
  m_fontSizes[size] = std::make_pair(width/length, height/length);
  
  SDL_FreeSurface(textSurface);
}