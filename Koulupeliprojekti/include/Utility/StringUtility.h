#pragma once
#include <vector>
#include <string>

namespace Utility
{
  std::vector<std::string> Tokenize(std::string text, std::string delimiters);
  std::vector<std::string> HandleControlCharacters(std::string line);
}