#include "VM/ScriptLoader.h"
#include "VM/MemoryManager.h"
#include "VM/VMState.h"
#include "VM/VMFunction.h"
#include "Utility/StringUtility.h"

// todo - move to separate file
#define STATICS_TOKEN "statics"
#define STRING_TOKEN "string"
#define INTEGER_TOKEN "integer"
#define NATIVE_POINTER_TOKEN "native_pointer"
#define AS_TOKEN "as"

#define END_STATICS_TOKEN "endstatics"


#define FUNCTION_TOKEN "function"
#define LOCALS_TOKEN "locals"
#define END_LOCALS_TOKEN "endlocals"

#define PUSH_INTEGER_TOKEN "PushInteger"
#define LOAD_STATIC_TOKEN "LoadStatic"
#define STORE_STATIC_TOKEN "StoreStatic"
#define INVOKE_NATIVE_TOKEN "InvokeNative"
#define JUMP_IF_ZERO_TOKEN "JumpIfZero"
#define JUMP_IF_NEGATIVE_TOKEN "JumpIfNegative"
#define JUMP_IF_POSITIVE_TOKEN "JUMPIfPositive"
#define RETURN_TOKEN "Return"
#define LABEL_TOKEN "Label"
#define STORE_LOCAL_TOKEN "StoreLocal"
#define LOAD_LOCAL_TOKEN "LoadLocal"

#define INTEGER_SUBTRACT_TOKEN "IntegerSubtract"

#define DOUBLE_TO_INTEGER_TOKEN "DoubleToInteger"

#define END_FUNCTION_TOKEN "endfunction"


void ExpectTokenCount(const std::vector<std::string> &tokens, const int count) {
  if (tokens.size() != count) {
    throw std::runtime_error("Invalid token count. Expected " + std::to_string(count) + " but was actually " + std::to_string(tokens.size()));
  }
}
void ExpectToken(const std::string &token, const std::string &expected) {
  if (token != expected) {
    throw std::runtime_error("Unexpected token '" + token + "'. Expected token '" + expected + "'");
  }
}

int32_t ConvertToInteger(const std::string &str) {
  try {
    return static_cast<int32_t>(std::stoi(str));
  } catch (const std::out_of_range &ex)  {
    throw std::runtime_error(str + " is too large to fit the inbuilt data type (32 bit signed integer)");
  } catch (const std::invalid_argument &ex) {
    throw std::runtime_error("'" + str + "'" + " is not a number");
  }
}

ScriptLoader::ScriptLoader(VMState &state, std::string path) : m_state(state), m_path(path), m_line(0) {

}

void ScriptLoader::Load() {
  m_scriptFile.open(m_path);
  if (!m_scriptFile.is_open()) {
    throw std::runtime_error("Could not open script file '" + m_path + "'");
  }


  try {
    LoadStatics();
    LoadFunctions();
  }
  catch (std::exception &ex) {
    std::string err = ex.what();
    throw std::runtime_error("An error occurred while processing script file '" + m_path + "' at line " + std:: to_string(m_line) + ": " + err);
  }

}

void ScriptLoader::LoadStatics() {
  std::string line;
  GetLine(line);
  if (line.empty()) {
    return;
  }

  ExpectToken(line, STATICS_TOKEN);

  while (GetLine(line)) {
    auto tokens = Utility::Tokenize(line, " ");
    if (tokens[0] == END_STATICS_TOKEN) {
      return;
    }

    AddStaticValue(tokens);

  }

}


void ScriptLoader::AddStaticValue(const std::vector<std::string> &tokens) {
  ExpectTokenCount(tokens, 4);
  ExpectToken(tokens[2], AS_TOKEN);
  VMValue obj;
  
  auto name = tokens[3];
  auto value = tokens[1];
  
  if (m_staticNameIndexMapping.find(name) != m_staticNameIndexMapping.end()) {
    throw std::runtime_error("Multiple declarations of static value " + name);
  }
  
  if (tokens[0] == STRING_TOKEN) {
    obj = MemMgrInstance().AllocateArray(ValueType::CHAR, value.length());
    MemMgrInstance().WriteToArrayIndex(obj, &value[0], 0, value.length());

  } else if (tokens[0] == INTEGER_TOKEN) {
    obj = VMValue{ ConvertToInteger(value) };
    
  }
  else if (tokens[0] == NATIVE_POINTER_TOKEN) {
    obj.set_native_pointer(nullptr);
  }
  else {
    throw std::runtime_error("Unexpected token " + tokens[0] + ". Expected type declaration");
  }

  auto index = m_state.AddStaticObject(obj);
  m_staticNameIndexMapping[name] = index;
}


void ScriptLoader::LoadFunctions() {
  std::string line;
  while (GetLine(line)) {
    if (line.empty()) {
      continue;
    }

    auto tokens = Utility::Tokenize(line, " ");
    ExpectToken(tokens[0], FUNCTION_TOKEN);
    ExpectTokenCount(tokens, 2);

    LoadFunction(tokens[1]);
  }
}

void ScriptLoader::LoadFunction(const std::string &name) {
  VMFunction function;
  function.SetName(name);
  
  auto locals = LoadLocals();
  
  std::unordered_map<std::string, size_t> labelPositions; // targets for jump instructions

  // contains the jumps where label was not yet processed (jumping forward)
  // name - placeholder bytecode location
  std::unordered_map<std::string, size_t> unhandledJumps; 

  std::string line;
  while (GetLine(line)) {
    if (line.empty()) {
      continue;
    }
    
    if (line == END_FUNCTION_TOKEN) {
      break;
    }


    // TODO - CLEANUP
    auto tokens = Utility::Tokenize(line, " ");

    if (tokens[0] == PUSH_INTEGER_TOKEN) {
      ExpectTokenCount(tokens, 2);
      function.AddByteCode(ByteCode::PUSH_INTEGER);
      function.AddByteCode(static_cast<ByteCode>(ConvertToInteger(tokens[1])));
      
    } else if (tokens[0] == LOAD_STATIC_TOKEN) {
      ExpectTokenCount(tokens, 2);
      if (m_staticNameIndexMapping.find(tokens[1]) == m_staticNameIndexMapping.end()) {
        throw std::runtime_error("Could not find registered static with name " + tokens[1]);
      }
      function.AddByteCode(ByteCode::LOAD_STATIC_OBJECT);
      function.AddByteCode(static_cast<ByteCode>(m_staticNameIndexMapping[tokens[1]]));

    } else if (tokens[0] == STORE_STATIC_TOKEN) {
      ExpectTokenCount(tokens, 2);
      if (m_staticNameIndexMapping.find(tokens[1]) == m_staticNameIndexMapping.end()) {
        throw std::runtime_error("Could not find registered static with name " + tokens[1]);
      }
      function.AddByteCode(ByteCode::STORE_STATIC_OBJECT);
      function.AddByteCode(static_cast<ByteCode>(m_staticNameIndexMapping[tokens[1]]));

    } else if (tokens[0] == JUMP_IF_ZERO_TOKEN) {
      ExpectTokenCount(tokens, 2);
      function.AddByteCode(ByteCode::JUMP_IF_ZERO);
      if (labelPositions.find(tokens[1]) == labelPositions.end()) {
        auto index = function.AddByteCode(ByteCode::NOP);
        unhandledJumps[tokens[1]] = index;
      } else {
        function.AddByteCode(static_cast<ByteCode>(labelPositions[tokens[1]]));
      }

    } else if (tokens[0] == JUMP_IF_NEGATIVE_TOKEN) {
      ExpectTokenCount(tokens, 2);
      function.AddByteCode(ByteCode::JUMP_IF_NEGATIVE);
      if (labelPositions.find(tokens[1]) == labelPositions.end()) {
        auto index = function.AddByteCode(ByteCode::NOP);
        unhandledJumps[tokens[1]] = index;
      }
      else {
        function.AddByteCode(static_cast<ByteCode>(labelPositions[tokens[1]]));
      }

    }
    else if (tokens[0] == JUMP_IF_POSITIVE_TOKEN) {
      ExpectTokenCount(tokens, 2);
      function.AddByteCode(ByteCode::JUMP_IF_POSITIVE);
      if (labelPositions.find(tokens[1]) == labelPositions.end()) {
        auto index = function.AddByteCode(ByteCode::NOP);
        unhandledJumps[tokens[1]] = index;
      }
      else {
        function.AddByteCode(static_cast<ByteCode>(labelPositions[tokens[1]]));
      }

    }
    else if (tokens[0] == INVOKE_NATIVE_TOKEN) {
      ExpectTokenCount(tokens, 1);
      function.AddByteCode(ByteCode::INVOKE_NATIVE);
    } else if (tokens[0] == RETURN_TOKEN) {
      ExpectTokenCount(tokens, 1);
      function.AddByteCode(ByteCode::RETURN);
    } else if (tokens[0] == LABEL_TOKEN) {
      ExpectTokenCount(tokens, 2);
      labelPositions[tokens[1]] = function.GetByteCodeCount();
    } else if (tokens[0] == STORE_LOCAL_TOKEN) {
      ExpectTokenCount(tokens, 2);
      if (locals.find(tokens[1]) == locals.end()) {
        throw std::runtime_error("No local variable '" + tokens[1] + "' defined");
      }
      function.AddByteCode(ByteCode::STORE_LOCAL);
      function.AddByteCode(static_cast<ByteCode>(locals[tokens[1]]));

    } else if (tokens[0] == LOAD_LOCAL_TOKEN) {
      ExpectTokenCount(tokens, 2);
      if (locals.find(tokens[1]) == locals.end()) {
        throw std::runtime_error("No local variable '" + tokens[1] + "' defined");
      }
      function.AddByteCode(ByteCode::LOAD_LOCAL);
      function.AddByteCode(static_cast<ByteCode>(locals[tokens[1]]));
    } else if (tokens[0] == INTEGER_SUBTRACT_TOKEN) {
      ExpectTokenCount(tokens, 1);
      function.AddByteCode(ByteCode::SUB_INTEGER);
    } else if (tokens[0] == DOUBLE_TO_INTEGER_TOKEN) {
      ExpectTokenCount(tokens, 1);
      function.AddByteCode(ByteCode::DOUBLE_TO_INTEGER);
    }
    else  {
      throw std::runtime_error("Unexpected token '" + tokens[0] + "'. Expected a command");
    }


  }

  HandleUnhandledJumps(function, unhandledJumps, labelPositions);


  function.SetLocalCount(locals.size());
  function.AddByteCode(ByteCode::RETURN);
  m_state.AddFunction(name, function);
}


void ScriptLoader::HandleUnhandledJumps(VMFunction &function, std::unordered_map<std::string, size_t> &unhandledJumps, std::unordered_map<std::string, size_t> &labelPositions)
{
  for (auto pair : unhandledJumps) {
    if (labelPositions.find(pair.first) == labelPositions.end()) {
      throw std::runtime_error("Could not find label '" + pair.first + "'");
    }
    function.ChangeByteCode(pair.second, static_cast<ByteCode>(labelPositions[pair.first]));
  }
}



std::unordered_map<std::string, size_t> ScriptLoader::LoadLocals()
{
  
  std::string line;
  GetLine(line);
  ExpectToken(line, LOCALS_TOKEN);
  std::unordered_map<std::string, uint32_t> localNameToIndexMappings;
  size_t localPos = 0;
  while (GetLine(line)) {
    if (line.empty()) {
      continue;
    }
    if (line == END_LOCALS_TOKEN) {
      break;
    }

    auto tokens = Utility::Tokenize(line, " ");
    ExpectTokenCount(tokens, 1);

    if (localNameToIndexMappings.find(tokens[0]) != localNameToIndexMappings.end()) {
      throw std::runtime_error("Local variable " + tokens[0] + " has already been defined");
    }
    localNameToIndexMappings[tokens[0]] = localPos++;
  }
  return localNameToIndexMappings;
}



bool ScriptLoader::GetLine(std::string &line) {
  
  if (!std::getline(m_scriptFile, line)) {
    return false;
  }
  ++m_line;
  line = line.substr(0, line.find("#")); // strip comments, if any
  line = Utility::Trim(line);

  return true;
}

