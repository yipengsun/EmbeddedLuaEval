# Author: Yipeng Sun
# Last Change: Fri Jan 07, 2022 at 10:51 PM +0100

VPATH := include:src:bin
HEADERS := $(shell find ./include -name "*.h")

# Compiler settings
COMPILER	:=	${CXX}
CXXFLAGS	:=	-pthread -std=c++17 -m64 -O2 -march=native -mtune=native -Iinclude
LINKFLAGS	:=	-llua -lyaml-cpp

.PHONY: all clean run-demo

all: clean

clean:
	@rm -rf ./bin/*

run-demo: demo params/simple.yml
	bin/demo -g "area" -i ./params/simple.yml
	bin/demo -g "r" -i ./params/simple.yml -t int
	bin/demo -g "name" -i ./params/simple.yml -t string
	bin/demo -g "isCircle" -i ./params/simple.yml -t bool

%: %.cpp flake.nix $(HEADERS)
	$(COMPILER) $(CXXFLAGS) -o bin/$@ $< $(LINKFLAGS)
