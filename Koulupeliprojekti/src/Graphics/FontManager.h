#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include <string>
#include <utility>
class Renderer;
class FontManager
{
public:
  FontManager();
  ~FontManager();

  static FontManager &Instance();

  void Initialize(std::string pathAndName);
  
  double GetFontWidth(int fontSize);
  double GetFontHeight(int fontSize);
  SDL_Texture *GetTextSingleLine(Renderer *renderer, std::string text, int fontSize, SDL_Color color);
  SDL_Texture *GetTextSingleLine(Renderer *renderer, std::string text, int fontSize);


private:
  void OpenFontFile(int fontSize);
  void CalculateFontSize(int size);

  std::string m_fontFilePathAndName;
  std::unordered_map<int, TTF_Font *> m_fonts;
  std::unordered_map<int, std::pair<double, double>> m_fontSizes;
  
};