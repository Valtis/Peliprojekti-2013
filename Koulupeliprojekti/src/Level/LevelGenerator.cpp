#include "Level/LevelGenerator.h"

LevelGenerator::LevelGenerator() {}
LevelGenerator::~LevelGenerator() {}

std::vector<int> LevelGenerator::generateLevel(int a, int b)
{
  std::vector<int> p(a*b, 0);
  for (int i = 0; i < b; ++i) {
    p[b * 0 + i] = 1;
    p[b * (a-1) + i] = 1;
  }
  for (int i = 0; i < a; ++i) {
    p[b * i + 0] = 1;
    p[b * i + (b-1)] = 1;
  }
  return p;
}

