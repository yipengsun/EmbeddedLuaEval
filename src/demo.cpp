// Author: Yipeng Sun
// License: BSD 2-clause
// Last Change: Fri Jan 07, 2022 at 05:49 PM +0100

#include <cxxopts.hpp>
#include <iostream>
#include <lua.hpp>

using namespace std;

int main(int argc, char** argv) {
  cxxopts::Options argOpts("demo", "demo: embedding Lua in C++.");

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

  lua_pushstring(luaState, "123.4");
  lua_setglobal(luaState, "a");

  return 0;
}
