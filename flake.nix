{
  description = "A simple demo to embed Lua in a C++ program.";

  inputs = {
    nixpkgs-pointer.url = "github:yipengsun/nixpkgs-pointer";
    nixpkgs.follows = "nixpkgs-pointer/nixpkgs";
    flake-utils.follows = "nixpkgs-pointer/flake-utils";
  };

  outputs = { self, nixpkgs-pointer, nixpkgs, flake-utils }:
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
