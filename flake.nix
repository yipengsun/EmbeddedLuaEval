{
  description = "A simple demo to embed Lua in a C++ program.";

  inputs = {
    nixpkgs.url = "nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShell = pkgs.mkShell {
          name = "EmbeddedLuaEval";
          buildInputs = with pkgs; [
            clang-tools
            luaformatter
            lua
            libyamlcpp
            cxxopts
          ];
        };
      });
}
