$(CXX) = g++

all:
	$(CXX) -o ./build/transpiler -std=c++23 -O2 ./src/main.cpp ./include/raylib/raylib/lib/* -L./include/raylib/raylib/lib/ -L./include/raylib/raygui/src -lraylib

clean:
	rm -rf ./build/*