#include "VM/ScriptLoader/ByteCodeCreator/ByteCodeCreator.h"
#include "VM/ScriptLoader/Tokens.h"
#include "VM/ByteCode.h"
#include "VM/VMFunction.h"

// helper macro to reduce typing
#define HANDLER(NAME____) void NAME____(ByteCodeCreatorArguments args)

namespace ByteCodeCreator{
  
  HANDLER(PopToken) {
    ExpectTokenCount(args.tokens, 1);
    args.function.AddByteCode(ByteCode::POP);
  }

  HANDLER(PushIntegerToken) {
    ExpectTokenCount(args.tokens, 2);
    args.function.AddByteCode(ByteCode::PUSH_INTEGER);
    args.function.AddByteCode(static_cast<ByteCode>(ConvertToInteger(args.tokens[1])));
  }

  HANDLER(LoadStaticToken) {
    ExpectTokenCount(args.tokens, 2);
    ExpectNameExists(args.staticNameToIndexMappings, args.tokens[1]);
    args.function.AddByteCode(ByteCode::LOAD_STATIC_OBJECT);
    args.function.AddByteCode(static_cast<ByteCode>(args.staticNameToIndexMappings[args.tokens[1]]));
  }

  HANDLER(StoreStaticToken) {
    ExpectTokenCount(args.tokens, 2);
    ExpectNameExists(args.staticNameToIndexMappings, args.tokens[1]);
    args.function.AddByteCode(ByteCode::STORE_STATIC_OBJECT);
    args.function.AddByteCode(static_cast<ByteCode>(args.staticNameToIndexMappings[args.tokens[1]]));
  }

  HANDLER(LoadLocalToken) {
    ExpectTokenCount(args.tokens, 2);
    ExpectNameExists(args.localsToIndexMappings, args.tokens[1]);
    args.function.AddByteCode(ByteCode::LOAD_LOCAL);
    args.function.AddByteCode(static_cast<ByteCode>(args.localsToIndexMappings[args.tokens[1]]));
  }

  HANDLER(StoreLocalToken) {
    ExpectTokenCount(args.tokens, 2);
    ExpectNameExists(args.localsToIndexMappings, args.tokens[1]);
    args.function.AddByteCode(ByteCode::STORE_LOCAL);
    args.function.AddByteCode(static_cast<ByteCode>(args.localsToIndexMappings[args.tokens[1]]));
  }

  void SetJumpTarget(VMFunction &function, std::unordered_map<std::string, size_t> &labelPositions, 
    std::unordered_map<std::string, size_t> &unhandled_jumps, std::string labelName) {
    if (labelPositions.find(labelName) == labelPositions.end()) {
      auto index = function.AddByteCode(ByteCode::NOP);
      unhandled_jumps[labelName] = index;
    }
    else {
      function.AddByteCode(static_cast<ByteCode>(labelPositions[labelName]));
    }
  }

  HANDLER(JumpIfZeroToken) {
    ExpectTokenCount(args.tokens, 2);
    args.function.AddByteCode(ByteCode::JUMP_IF_ZERO);
    SetJumpTarget(args.function, args.labelPositions, args.unhandled_jumps, args.tokens[1]);
  }

  HANDLER(JumpIfNegativeToken) {
    ExpectTokenCount(args.tokens, 2);
    args.function.AddByteCode(ByteCode::JUMP_IF_NEGATIVE);
    SetJumpTarget(args.function, args.labelPositions, args.unhandled_jumps, args.tokens[1]);
  }

  HANDLER(JumpIfPositiveToken) {
    ExpectTokenCount(args.tokens, 2);
    args.function.AddByteCode(ByteCode::JUMP_IF_POSITIVE);
    SetJumpTarget(args.function, args.labelPositions, args.unhandled_jumps, args.tokens[1]);
  }

  HANDLER(LabelToken) {
    ExpectTokenCount(args.tokens, 2);
    args.labelPositions[args.tokens[1]] = args.function.GetByteCodeCount();
  }

  HANDLER(ReturnToken) {
    ExpectTokenCount(args.tokens, 1);
    args.function.AddByteCode(ByteCode::RETURN);
  }

  HANDLER(IntegerSubtractToken) {
    ExpectTokenCount(args.tokens, 1);
    args.function.AddByteCode(ByteCode::SUB_INTEGER);
  }

  HANDLER(AllocateIntegerArrayToken) {
    ExpectTokenCount(args.tokens, 1);
    args.function.AddByteCode(ByteCode::ALLOCATE_INTEGER_ARRAY);
  }

  HANDLER(InvokeNativeToken) {
    ExpectTokenCount(args.tokens, 1);
    args.function.AddByteCode(ByteCode::INVOKE_NATIVE);
  }

  HANDLER(DoubleToIntegerToken) {
    ExpectTokenCount(args.tokens, 1);
    args.function.AddByteCode(ByteCode::DOUBLE_TO_INTEGER);
  }

  std::unordered_map<std::string, ByteCodeCreatorFunction> CreateByteCodeCreators() {
    
    std::unordered_map<std::string, ByteCodeCreatorFunction> creators;
  
    // push/pops
    creators[POP_TOKEN] = &PopToken;
    creators[PUSH_INTEGER_TOKEN] = &PushIntegerToken;

    // loads/stores
    creators[LOAD_STATIC_TOKEN] = &LoadStaticToken;
    creators[STORE_STATIC_TOKEN] = &StoreStaticToken;
    creators[LOAD_LOCAL_TOKEN] = &LoadLocalToken;
    creators[STORE_LOCAL_TOKEN] = &StoreLocalToken;

    // jumps/labels/return
    creators[JUMP_IF_ZERO_TOKEN] = &JumpIfZeroToken;
    creators[JUMP_IF_NEGATIVE_TOKEN] = &JumpIfNegativeToken;
    creators[JUMP_IF_POSITIVE_TOKEN] = &JumpIfPositiveToken;
    creators[LABEL_TOKEN] = &LabelToken;
    creators[RETURN_TOKEN] = &ReturnToken;
    
    // arithmetic operations
    creators[INTEGER_SUBTRACT_TOKEN] = &IntegerSubtractToken;

    // allocations
    creators[ALLOCATE_INTEGER_ARRAY_TOKEN] = &AllocateIntegerArrayToken;
    
    // function invocations
    creators[INVOKE_NATIVE_TOKEN] = &InvokeNativeToken;

    // conversions
    creators[DOUBLE_TO_INTEGER_TOKEN] = &DoubleToIntegerToken;

    
    
    return creators;
  }


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

  void ExpectNameExists(const std::unordered_map<std::string, size_t> &mapping, const std::string &name) {
    if (mapping.find(name) == mapping.end()) {
      throw std::runtime_error("Could not find name " + name);
    }
  }


  int32_t ConvertToInteger(const std::string &str) {
    try {
      return static_cast<int32_t>(std::stoi(str));
    }
    catch (const std::out_of_range &ex)  {
      throw std::runtime_error(str + " is too large to fit the inbuilt data type (32 bit signed integer)");
    }
    catch (const std::invalid_argument &ex) {
      throw std::runtime_error("'" + str + "'" + " is not a number");
    }
  }


}

