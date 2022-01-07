// Author: Yipeng Sun
// License: BSD 2-clause
// Last Change: Fri Jan 07, 2022 at 10:06 PM +0100

#include <cxxopts.hpp>
#include <iostream>

#include "param_eval.h"

using namespace std;

int main(int argc, char** argv) {
  cxxopts::Options argOpts("demo", "a demo on embedding Lua in C++.");

  // clang-format off
  argOpts.add_options()
    ("h,help", "print usage")
    ("g,get", "get value of a known variable", cxxopts::value<string>())
    ("i,inputYml", "input YAML file", cxxopts::value<string>())
  ;
  // clang-format on

  auto parsedArgs = argOpts.parse(argc, argv);
  if (parsedArgs.count("help")) {
    cout << argOpts.help() << endl;
    return 0;
  }

  auto exe = make_unique<ParamEval>();
  exe->set("a", "3.14*5");

  auto val = exe->get<double>(parsedArgs["get"].as<string>());
  cout << "val = " << val << endl;

  return 0;
}
