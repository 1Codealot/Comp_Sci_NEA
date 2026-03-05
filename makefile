$(CXX) = g++

all:
	xxd -i ./font/JetBrainsMono-Bold.ttf > src/font.hpp # To be able to link the font into the binary.
	$(CXX) -o ./build/transpiler -std=c++23 -O2 ./src/main.cpp ./include/raylib/raylib/lib/* -L./include/raylib/raylib/lib/ -L./include/raylib/raygui/src -lraylib

clean:
	rm src/font.hpp
	rm -rf ./build/*