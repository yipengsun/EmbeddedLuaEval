# EmbededLuaEval [![github CI](https://github.com/yipengsun/EmbededLuaEval/workflows/CI/badge.svg?branch=master)](https://github.com/yipengsun/EmbededLuaEval/actions?query=workflow%3ACI)

[![built with nix](https://builtwithnix.org/badge.svg)](https://builtwithnix.org)

A simple demo to embed Lua in a C++ program.

**WARNING**: The Lua expressions are **NOT** checked against arbitrary code
injection! **Sanitize the input YAML files** to ensure no malicious code before
loading them in!


## Motivation

Sometimes we want to define a lot of variables (O(1000)) in C++ and be able to
tweak them easily without a recompilation.

A naive attempt would be just define these values in YAML and load them in C++.
However, in a more realistic case, those values may depend on previously
defined values s.t. writing them all explicitly in YAML can be a pain.

For example:

```yml
pi: 3.14
r: 3
area: pi*r^2
circumference: 2*pi*r
d: math.sqrt(area/pi)*2
```

It would be a pain to manually compute `area,circumference,d`.

Therefore, a Lua interpreter is embedded in C++ so that these values can be
defined just like above, without explicit computation on user's side.


## Try out this project

1. Install `nix` with flake support
2. Run `nix develop`
3. In the resulting shell, run `make`
4. Profit!
