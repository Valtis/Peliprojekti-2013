#include "Utility/StringUtility.h"
#include <cstring>

std::vector<std::string> Utility::Tokenize(std::string text, std::string delimiters)
{
  std::vector<std::string> tokens;
  if (text.size() == 0)
  {
    return tokens;
  }

  char *retVal;
  const unsigned int textbufferSize = text.length() + 1;
  char *textBuffer = new char[textbufferSize];
  for (unsigned int i = 0; i < textbufferSize; ++i)
  {
    textBuffer[i] = '\0';
  }
#ifdef _MSC_VER
  strncpy_s(textBuffer, textbufferSize, text.c_str(), text.length());
  char *nextToken = nullptr;
  retVal = strtok_s(textBuffer, delimiters.c_str(), &nextToken);
#else
  strncpy(textBuffer, text.c_str(), text.length());
  retVal = strtok(textBuffer, delimiters.c_str());
#endif



  while (retVal != nullptr)
  {
    tokens.push_back(retVal);
#ifdef _MSC_VER
    retVal = strtok_s(nullptr, delimiters.c_str(), &nextToken);
#else
    retVal = strtok(nullptr, delimiters.c_str());
#endif
  }

  delete [] textBuffer;
  return tokens;
}

std::vector<std::string> Utility::HandleControlCharacters(std::string line)
{
  std::vector<std::string> tokens = Tokenize(line, "\n");
 
  return tokens;
}


std::vector<std::string> Utility::WrapLines(std::vector<std::string> lines, double lineWidth, double characterWidth)
{
  std::vector<std::string> wrappedLines;
  for (auto line : lines)
  {
    auto lineTokens = Tokenize(line, " ");
    std::string newLine;

    for (auto token : lineTokens)
    {
      if ((newLine + " " + token).length()*characterWidth < lineWidth)
      {
        if (!newLine.empty())
        {
          newLine += " ";
        }
        newLine += token;
      }
      else
      {
        wrappedLines.push_back(newLine);
        newLine = token;
      }
    }

    if (!newLine.empty())
    {
      wrappedLines.push_back(newLine);
    }
  }

  return wrappedLines;
}