// Author: Yipeng Sun
// License: BSD 2-clause
// Last Change: Fri Jan 07, 2022 at 06:30 PM +0100

#include <iostream>

#include <cxxopts.hpp>
#include <lua.hpp>

using namespace std;

int main(int argc, char** argv) {
  cxxopts::Options argOpts("demo", "a demo on embedding Lua in C++.");

  // clang-format off
  argOpts.add_options()
    ("h,help", "print usage")
    ("e,eval", "evaluate expression", cxxopts::value<string>())
    ("i,inputYml", "input YAML file", cxxopts::value<string>())
  ;
  // clang-format on

  auto parsedArgs = argOpts.parse(argc, argv);
  if (parsedArgs.count("help")) {
    cout << argOpts.help() << endl;
    return 0;
  }

  auto luaState = luaL_newstate();
  luaL_openlibs(luaState);

  lua_getglobal(luaState, "loadstring");
  lua_pushstring(luaState, "return 123.4+7.9");
  lua_call(luaState, 1, 1);
  lua_call(luaState, 0, 1);
  lua_setglobal(luaState, "a");

  auto a = lua_tonumber(luaState, 0);  // latest value on top of the stack

  lua_settop(luaState, 0);  // avoid stack overflow
  lua_getglobal(luaState, "loadstring");
  lua_pushstring(luaState, "return a*2");
  lua_call(luaState, 1, 1);
  lua_call(luaState, 0, 1);
  lua_setglobal(luaState, "b");

  auto b = lua_tonumber(luaState, 0);

  lua_settop(luaState, 0);  // avoid stack overflow
  lua_getglobal(luaState, "a");
  auto a2 = lua_tonumber(luaState, 1);

  cout << a << " " << b << " " << a2 << endl;

  lua_close(luaState);
  return 0;
}
