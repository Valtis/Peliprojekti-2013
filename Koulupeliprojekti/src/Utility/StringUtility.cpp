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
  std::vector<std::string> lines;
  
  int tabSize = 4;
  for (auto token: tokens)
  {
    std::string newLine;
    for (unsigned int i = 0; i < token.length(); ++i)
    {
      if (token[i] == '\t')
      {
        for (int j = (newLine.length() % tabSize); j < tabSize; ++j)
          newLine += " ";
      }
      else
        newLine += token[i];
    }

    if (newLine.length() > 0)
      lines.push_back(newLine);
    newLine = "";
  }

  return lines;
}

