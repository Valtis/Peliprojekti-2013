#include "UI/Window/TextField.h"
#include "UI/Window/TextElement.h"
#include "Utility/StringUtility.h"
#include "Graphics/FontManager.h"
TextField::TextField(std::string text, int fontSize, SDL_Rect location, Renderer *renderer)
{
  std::vector<std::string> lines = Utility::HandleControlCharacters(text);
  lines = Utility::WrapLines(lines, location.w, FontManager::Instance().GetFontWidth(fontSize));

  int spaceBetweenLines = 3;
  int distanceToLeftEdge = 3;
  int fontHeight = FontManager::Instance().GetFontHeight(fontSize);
  
  auto lineIter = lines.begin();
  for (int y = 0; y < location.h - fontHeight && lineIter != lines.end(); y += fontHeight + spaceBetweenLines, ++lineIter)
  {
    SDL_Rect textLocation = { location.x + distanceToLeftEdge, y, location.w, fontHeight + spaceBetweenLines  };
    std::unique_ptr<TextElement> line(new TextElement(*lineIter, fontSize, TextAlingment::ALIGN_LEFT, textLocation, renderer));
    AddWindow(std::move(line));
  }
}