// Author: Yipeng Sun
// License: BSD 2-clause
// Last Change: Sat Jan 08, 2022 at 02:59 AM +0100

#ifndef _LUA_DEMO_PARAM_EVAL_
#define _LUA_DEMO_PARAM_EVAL_

#include <iostream>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>
#include <lua.hpp>

using std::cout;
using std::endl;

using std::string;
using std::vector;

////////////////
// Error code //
////////////////

#define KEY_NOT_FOUND 1
#define KEY_EXIST 2
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
  lua_State*         mLuaInst = nullptr;
  vector<YAML::Node> mLoadedYmls;
  vector<string>     mKnownVars;

  // Lua modules to blacklist
  vector<string> mBlackList = {"io",        "os",      "dofile", "loadfile",
                               "coroutine", "package", "debug"};

  void getCommon(string const name);
  bool fileExist(string const file);
  void blackListLuaModule(vector<string> modules);
};

// Constructor/destructor //////////////////////////////////////////////////////

ParamEval::ParamEval(bool sandbox) {
  mLuaInst = luaL_newstate();
  luaL_openlibs(mLuaInst);

  if (sandbox) blackListLuaModule(mBlackList);
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
  if (std::find(mKnownVars.begin(), mKnownVars.end(), name) ==
      mKnownVars.end()) {
    // cout << "Variable " << name << " has already been defined!" << endl;
    // exit(KEY_EXIST);
    mKnownVars.emplace_back(name);
  }

  lua_settop(mLuaInst, 0);          // to avoid stack overflow
  lua_getglobal(mLuaInst, "load");  // the magic 'eval' in Lua
  lua_pushstring(mLuaInst, ("return " + expr).c_str());
  lua_call(mLuaInst, 1, 1);  // call "loadstring(expr) -> lambda: any"
  lua_call(mLuaInst, 0, 1);
  lua_setglobal(mLuaInst, name.c_str());
}

// Private: helpers ////////////////////////////////////////////////////////////

void ParamEval::getCommon(string const name) {
  if (std::find(mKnownVars.begin(), mKnownVars.end(), name) ==
      mKnownVars.end()) {
    cout << "Unknown variable name: " << name << endl;
    exit(KEY_NOT_FOUND);
  }
  lua_settop(mLuaInst, 0);
  lua_getglobal(mLuaInst, name.c_str());
}

bool ParamEval::fileExist(string file) {
  if (FILE* _file = fopen(file.c_str(), "r")) {
    fclose(_file);
    return true;
  } else
    return false;
}

void ParamEval::blackListLuaModule(vector<string> modules) {
  for (const auto& m : modules) {
    lua_pushnil(mLuaInst);
    lua_setglobal(mLuaInst, m.c_str());
  }
}

#endif
