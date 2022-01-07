# Author: Yipeng Sun
# Last Change: Fri Jan 07, 2022 at 11:46 PM +0100

VPATH := include:src:bin
HEADERS := $(shell find ./include -name "*.h")

# Compiler settings
COMPILER	:=	${CXX}
CXXFLAGS	:=	-pthread -std=c++17 -m64 -O2 -march=native -mtune=native -Iinclude
LINKFLAGS	:=	-llua -lyaml-cpp

.PHONY: clean run-demo

run-demo: demo params/simple.yml
	bin/demo -g area,r,name -t double,double,string -i ./params/simple.yml
	bin/demo -g isCircle,wtf1,wtf2 -t bool,double,double -i ./params/simple.yml

clean:
	@rm -rf ./bin/*

%: %.cpp flake.nix $(HEADERS)
	$(COMPILER) $(CXXFLAGS) -o bin/$@ $< $(LINKFLAGS)
