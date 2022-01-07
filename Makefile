# Author: Yipeng Sun
# Last Change: Fri Jan 07, 2022 at 09:19 PM +0100

VPATH := include:src:bin
HEADERS := $(shell find ./include -name "*.h")

# Compiler settings
COMPILER	:=	${CXX}
CXXFLAGS	:=	-pthread -std=c++17 -m64 -O2 -march=native -mtune=native -Iinclude
LINKFLAGS	:=	-llua -lyaml-cpp

.PHONY: all clean

all: clean

clean:
	@rm -rf ./bin/*

run-demo: demo
	bin/demo "1+1"

%: %.cpp flake.nix $(HEADERS)
	$(COMPILER) $(CXXFLAGS) -o bin/$@ $< $(LINKFLAGS)
