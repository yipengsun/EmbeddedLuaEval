# EmbededLuaEval [![github CI](https://github.com/yipengsun/EmbededLuaEval/workflows/CI/badge.svg?branch=master)](https://github.com/yipengsun/EmbededLuaEval/actions?query=workflow%3ACI)

[![built with nix](https://builtwithnix.org/badge.svg)](https://builtwithnix.org)

A simple demo to embed Lua in a C++ program.


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


## On security

By default the embedded Lua interpreter is running in a sandboxed mode, where
most of the Lua standard libraries are unloaded so that IO and system calls are
forbidden.

Suppose the following malicious YAML file:

```yml
pi: math.pi
warning: >-
    os.execute('echo "Sandbox your Lua!! Next time I will be calling rm -rf!"')
```

Run with the sandboxed Lua:
```shell
> bin/demo -g pi -t double -i ./params/lalaland.yml
PANIC: unprotected error in call to Lua API ([string "return os.execute('echo "Sandbox your Lua!! N..."]:1: attempt to index global 'os' (a nil value))
```

If you feel particularly brave:
```shell
> bin/demo -g pi -t double -i ./params/lalaland.yml --sandbox=false
Sandbox your Lua!! Next time I will be calling rm -rf!
Parameter "pi" = 3.14159
```
