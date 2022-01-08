# Author: Yipeng Sun
# Last Change: Sat Jan 08, 2022 at 02:51 AM +0100

VPATH := include:src:bin
HEADERS := $(shell find ./include -name "*.h")

# Compiler settings
COMPILER	:=	${CXX}
CXXFLAGS	:=	-pthread -std=c++17 -m64 -O2 -march=native -mtune=native -Iinclude
LINKFLAGS	:=	-llua -lyaml-cpp

.PHONY: clean run-demo long-yml-gen

run-demo: demo params/simple.yml
	bin/demo -g area,r,name,d,rightAngleInRad -t double,double,string,double,double -i ./params/simple.yml
	bin/demo -g isCircle,wtf1,wtf2 -t bool,double,double -i ./params/simple.yml
	bin/demo -g val1,val100,val314,val9999 -t double,double,double,double -i ./params/long.yml
	bin/demo -g pi -t double -i ./params/lalaland.yml

long-yml-gen:
	scripts/make_large_test_yml.lua 9999 ./params/long.yml

clean:
	@rm -rf ./bin/*

%: %.cpp flake.nix $(HEADERS)
	$(COMPILER) $(CXXFLAGS) -o bin/$@ $< $(LINKFLAGS)
