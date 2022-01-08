// Author: Yipeng Sun
// License: BSD 2-clause
// Last Change: Sat Jan 08, 2022 at 03:11 PM +0100

#ifndef _LUA_DEMO_PARAM_EVAL_
#define _LUA_DEMO_PARAM_EVAL_

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>
#include <lua.hpp>

using std::cout;
using std::endl;

using std::map;
using std::string;
using std::vector;

////////////////
// Error code //
////////////////

#define KEY_NOT_FOUND 1
#define KEY_EXIST 2
#define KEY_PROTECTED 3
#define FILE_NOT_FOUND 12
#define UNSUPPORTED_TYPE 21

//////////////////////////////////////////////////////////////////
// Load parameters from YAML files and be able to evaluate them //
//////////////////////////////////////////////////////////////////

class ParamEval {
 public:
  ParamEval(bool sandbox);
  ParamEval() : ParamEval(true){};
  ~ParamEval();

  // Common
  void loadParam(string const file);
  void set(string const name, string const expr);
  template <typename T>
  T get(string const name);

  // Mostly debug
  vector<string> getKnownVars() { return mKnownVars; };

 private:
  bool               mSandbox;
  lua_State*         mLuaInst = nullptr;
  vector<YAML::Node> mLoadedYmls;
  vector<string>     mKnownVars;

  // Lua modules to blacklist
  vector<string> mBlackList = {"io",        "os",      "dofile", "loadfile",
                               "coroutine", "package", "debug",  "require"};
  map<string, string> mAllowedInSandbox = {{"math", "math"},  // name, SB name
                                           {"print", "print"},
                                           {"pairs", "pairs"}};
  vector<string>      mProtectedVars    = {"_mSandboxEnv"};

  void getCommon(string const name);
  void setNoSandbox(string const name, string const expr);
  void setSandbox(string const name, string const expr);

  void blackListLuaModule();
  void makeSandboxEnv();
  void pushVarToSandbox(string const varName, string const varNameInSandbox);
  void pushVarToSandbox(string const varName) {
    pushVarToSandbox(varName, varName);
  };

  bool fileExist(string const file);
};

// Constructor/destructor //////////////////////////////////////////////////////

ParamEval::ParamEval(bool sandbox) : mSandbox(sandbox) {
  mLuaInst = luaL_newstate();
  luaL_openlibs(mLuaInst);

  if (mSandbox) {
    blackListLuaModule();  // Baseline blacklist to ensure no un-restricted Lua
                           // interpreter is exposed
    makeSandboxEnv();
  }
}

ParamEval::~ParamEval() { lua_close(mLuaInst); }

// Public: loaders  ////////////////////////////////////////////////////////////

void ParamEval::loadParam(const string file) {
  if (!fileExist(file)) {
    cout << "YAML file " << file << " doesn't exist!" << endl;
    exit(FILE_NOT_FOUND);
  }

  auto loadedYml = YAML::LoadFile(file);
  mLoadedYmls.emplace_back(loadedYml);

  for (auto it = loadedYml.begin(); it != loadedYml.end(); it++) {
    set(it->first.as<string>(), it->second.as<string>());
  }
}

// Public //////////////////////////////////////////////////////////////////////
// FIXME: These are NOT thread-safe, as multiple 'eval's can definitely mess up
//        with the stack! But we don't care about that for now.

template <>
int ParamEval::get<int>(string const name) {
  getCommon(name);
  return lua_tointeger(mLuaInst, 1);
}

template <>
double ParamEval::get<double>(string const name) {
  getCommon(name);
  return lua_tonumber(mLuaInst, 1);
}

template <>
bool ParamEval::get<bool>(string const name) {
  getCommon(name);
  return lua_toboolean(mLuaInst, 1);
}

template <>
string ParamEval::get<string>(string const name) {
  getCommon(name);
  return lua_tostring(mLuaInst, 1);
}

template <typename T>
T ParamEval::get(string const name) {
  cout << "Unsupported type: " << typeid(T).name() << endl;
  exit(UNSUPPORTED_TYPE);
}

void ParamEval::set(string const name, string const expr) {
  if (mSandbox)
    setSandbox(name, expr);
  else
    setNoSandbox(name, expr);
}

// Private  ////////////////////////////////////////////////////////////////////

void ParamEval::getCommon(string const name) {
  if (std::find(mKnownVars.begin(), mKnownVars.end(), name) ==
      mKnownVars.end()) {
    cout << "Unknown variable name: " << name << endl;
    exit(KEY_NOT_FOUND);
  }
  lua_settop(mLuaInst, 0);
  lua_getglobal(mLuaInst, name.c_str());
}

void ParamEval::setNoSandbox(string const name, string const expr) {
  if (std::find(mKnownVars.begin(), mKnownVars.end(), name) ==
      mKnownVars.end()) {
    mKnownVars.emplace_back(name);
  }

  lua_settop(mLuaInst, 0);          // to avoid stack overflow
  lua_getglobal(mLuaInst, "load");  // the magic 'eval' in Lua
  lua_pushstring(mLuaInst, ("return " + expr).c_str());
  lua_call(mLuaInst, 1, 1);  // call "loadstring(expr) -> lambda: any"
  lua_call(mLuaInst, 0, 1);
  lua_setglobal(mLuaInst, name.c_str());
}

void ParamEval::setSandbox(string const name, string const expr) {
  if (std::find(mProtectedVars.begin(), mProtectedVars.end(), name) !=
      mProtectedVars.end()) {
    cout << "Forbidden: Trying to change a protected variable: " << name
         << endl;
    exit(KEY_PROTECTED);
  }

  if (std::find(mKnownVars.begin(), mKnownVars.end(), name) ==
      mKnownVars.end()) {
    mKnownVars.emplace_back(name);
  }

  lua_settop(mLuaInst, 0);          // to avoid stack overflow
  lua_getglobal(mLuaInst, "load");  // the magic 'eval' in Lua
  lua_pushstring(mLuaInst, ("return " + expr).c_str());
  lua_pushstring(mLuaInst, "");
  lua_pushstring(mLuaInst, "t");
  lua_getglobal(mLuaInst, "_mSandboxEnv");
  lua_call(mLuaInst, 4, 1);  // call "loadstring(expr) -> lambda: any"
  lua_call(mLuaInst, 0, 1);
  lua_setglobal(mLuaInst, name.c_str());

  // We need to make the newly defined variable in the sandbox
  pushVarToSandbox(name);
}

// Private: helpers ////////////////////////////////////////////////////////////

bool ParamEval::fileExist(string file) {
  if (FILE* _file = fopen(file.c_str(), "r")) {
    fclose(_file);
    return true;
  } else
    return false;
}

void ParamEval::blackListLuaModule() {
  for (const auto& m : mBlackList) {
    lua_pushnil(mLuaInst);
    lua_setglobal(mLuaInst, m.c_str());
  }
  lua_settop(mLuaInst, 0);
}

void ParamEval::makeSandboxEnv() {
  // Create a new empty table
  lua_newtable(mLuaInst);
  lua_setglobal(mLuaInst, "_mSandboxEnv");

  for (const auto& kv : mAllowedInSandbox) {
    pushVarToSandbox(kv.first, kv.second);
  }
  lua_settop(mLuaInst, 0);
}

void ParamEval::pushVarToSandbox(string const varName,
                                 string const varNameInSandbox) {
  lua_settop(mLuaInst, 0);
  lua_getglobal(mLuaInst, "_mSandboxEnv");   // table, stack -2
  lua_getglobal(mLuaInst, varName.c_str());  // variable, stack -1
  lua_setfield(mLuaInst, -2, varNameInSandbox.c_str());
}

#endif
