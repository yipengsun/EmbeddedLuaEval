# Author: Yipeng Sun
# Last Change: Fri Jan 07, 2022 at 03:15 PM +0100

VPATH := include:src:gen
HEADERS := $(shell find ./include -name "*.h")

# Compiler settings
COMPILER	:=	${CXX}
CXXFLAGS	:=	-O2 -march=native -mtune=native -Iinclude
LINKFLAGS	:=	-llua -lyaml-cpp

.PHONY: all clean

all: clean

clean:
	@rm -rf ./bin/*

%: %.cpp flake.nix $(HEADERS)
	$(COMPILER) $(CXXFLAGS) -o bin/$@ $< $(LINKFLAGS)
