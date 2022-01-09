# Author: Yipeng Sun
# Last Change: Sun Jan 09, 2022 at 03:01 AM +0100

VPATH := include:src:bin
HEADERS := $(shell find ./include -name "*.h")
PARAMS := $(shell find ./params -name "*.yml")

# Compiler settings
COMPILER	:=	${CXX}
CXXFLAGS	:=	-pthread -std=c++17 -m64 -O2 -march=native -mtune=native -Iinclude
LINKFLAGS	:=	-llua -lyaml-cpp

.PHONY: test clean run-demo benchmark long-yml-gen

test: run-demo benchmark

run-demo: demo $(PARAMS)
	@echo ""
	@echo "Basic usage"
	@echo "==========="
	bin/demo -g area,r,name,d,rightAngleInRad -t double,double,string,double,double -i ./params/simple.yml
	bin/demo -g isCircle,wtf1,wtf2 -t bool,double,double -i ./params/simple.yml
	@echo ""
	@echo "Run over a long YAML..."
	@echo "======================="
	bin/demo -g val1,val100,val314,val9999 -t double,double,double,double -i ./params/long.yml
	@echo ""
	@echo "Show the importance of sandboxing Lua"
	@echo "====================================="
	bin/demo -g pi -t double -i ./params/lalaland.yml || true
	bin/demo -g pi -t double -i ./params/lalaland.yml --sandbox=false
	@echo ""
	@echo "More advanced usage"
	@echo "==================="
	bin/demo -i ./params/advanced.yml

benchmark: demo params/long.yml
	@echo ""
	@echo "Benchmark demo with the long YAML with 10000 variables..."
	@echo "========================================================="
	time bin/demo -i ./params/long.yml

long-yml-gen:
	scripts/make_large_test_yml.lua 9999 ./params/long.yml

clean:
	@rm -rf ./bin/*

%: %.cpp flake.nix $(HEADERS)
	$(COMPILER) $(CXXFLAGS) -o bin/$@ $< $(LINKFLAGS)
