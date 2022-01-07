// Author: Yipeng Sun
// License: BSD 2-clause
// Last Change: Fri Jan 07, 2022 at 10:39 PM +0100

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
    ("t,type", "parameter type", cxxopts::value<string>()
     ->default_value("double"))
  ;
  // clang-format on

  auto parsedArgs = argOpts.parse(argc, argv);
  if (parsedArgs.count("help")) {
    cout << argOpts.help() << endl;
    return 0;
  }

  auto exe = make_unique<ParamEval>();
  exe->loadParam(parsedArgs["inputYml"].as<string>());

  auto paramType = parsedArgs["type"].as<string>();
  if (paramType == "double") {
    auto val = exe->get<double>(parsedArgs["get"].as<string>());
    cout << "Parameter = " << val << endl;
  } else if (paramType == "int") {
    auto val = exe->get<int>(parsedArgs["get"].as<string>());
    cout << "Parameter = " << val << endl;
  } else if (paramType == "bool") {
    auto val = exe->get<bool>(parsedArgs["get"].as<string>());
    cout << "Parameter = " << val << endl;
  } else if (paramType == "string") {
    auto val = exe->get<string>(parsedArgs["get"].as<string>());
    cout << "Parameter = " << val << endl;
  } else {
    cout << "Unsupported type: " << paramType << endl;
  }

  return 0;
}
