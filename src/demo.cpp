// Author: Yipeng Sun
// License: BSD 2-clause
// Last Change: Fri Jan 07, 2022 at 11:47 PM +0100

#include <cxxopts.hpp>
#include <iostream>

#include "param_eval.h"

using namespace std;

void printParamVal(ParamEval* const exe, const vector<string>& names,
                   const vector<string>& types) {
  for (int i = 0; i < names.size(); i++) {
    auto paramType = types[i];
    cout << "Parameter "
         << "\"" << names[i] << "\"";
    if (paramType == "double") {
      auto val = exe->get<double>(names[i]);
      cout << " = " << val << endl;
    } else if (paramType == "int") {
      auto val = exe->get<int>(names[i]);
      cout << " = " << val << endl;
    } else if (paramType == "bool") {
      auto val = exe->get<bool>(names[i]);
      cout << " = " << val << endl;
    } else if (paramType == "string") {
      auto val = exe->get<string>(names[i]);
      cout << " = " << val << endl;
    } else {
      cout << " has an unsupported type: " << paramType << endl;
    }
  }
}

int main(int argc, char** argv) {
  cxxopts::Options argOpts("demo", "a demo on embedding Lua in C++.");

  // clang-format off
  argOpts.add_options()
    ("h,help", "print usage")
    ("g,get", "get value of variables", cxxopts::value<vector<string>>())
    ("t,type", "variable types", cxxopts::value<vector<string>>())
    ("i,inputYml", "input YAML file", cxxopts::value<string>())
  ;
  // clang-format on

  auto parsedArgs = argOpts.parse(argc, argv);
  if (parsedArgs.count("help")) {
    cout << argOpts.help() << endl;
    return 0;
  }

  auto exe = make_unique<ParamEval>();
  exe->loadParam(parsedArgs["inputYml"].as<string>());

  const auto names = parsedArgs["get"].as<vector<string>>();
  const auto types = parsedArgs["type"].as<vector<string>>();
  printParamVal(exe.get(), names, types);

  return 0;
}
