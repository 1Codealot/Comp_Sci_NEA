#include <iostream>
#include <fstream>
#include "graphics.hpp"

// Dummy procedure just to demonstrate that I can run a function on a button press.
void compile(std::string path_to_input_file)
{
    std::ifstream input_file(path_to_input_file);
    std::string tmp;
    while (std::getline(input_file, tmp))
    {
        std::cout << tmp << std::endl;
    }
}

        char file_path[max_text_size] = "\0";
int main()
{
    startWindow();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);


       text_box((Rectangle){50,50,200,150}, "Enter path to input file", "", "Confirm", file_path);
        

        // Creates the button
        button compile_button((Rectangle){50, 200, 200, 50}, "Transpile!", compile, std::string(file_path));
        EndDrawing();
    }

    CloseWindow();
    return 0;
}