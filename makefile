all:
	g++ -o ./build/transpiler ./src/main.cpp ./include/raylib/raylib/lib/* -L./include/raylib/raylib/lib/ -L./include/raylib/raygui/src -lraylib

clean:
	rm -rf ./build/*