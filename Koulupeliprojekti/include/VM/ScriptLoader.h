#pragma once
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <unordered_map>

class VMFunction;
class VMState;


class ScriptLoader {
public:
  ScriptLoader(VMState &state, std::string path);

  void Load();
private:
  void LoadStatics();
  void LoadFunctions();
  void LoadFunction(const std::string &name);
  bool GetLine(std::string &line);
  void AddStaticValue(const std::vector<std::string> &tokens);
  std::unordered_map<std::string, size_t> LoadLocals();
  void HandleUnhandledJumps(VMFunction &function, std::unordered_map<std::string, size_t> &unhandledJumps, std::unordered_map<std::string, size_t> &labelPositions);
  VMState &m_state;
  std::string m_path;
  uint32_t m_line;
  std::ifstream m_scriptFile;

  std::unordered_map<std::string, size_t> m_staticNameIndexMapping;
};