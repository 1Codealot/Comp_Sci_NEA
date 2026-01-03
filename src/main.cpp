#include <iostream>
#include <fstream>
#include "graphics.hpp"
#include "tokeniser.hpp"
#include "analysis.hpp"

std::string log_text;
// Dummy procedure just to demonstrate that I can run a function on a button press.
void compile(std::string path_to_input_file)
{
    log_text = ""; // Reset the log
    std::ifstream input_file(path_to_input_file);
    std::string code((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

    std::vector<std::string> tokens = tokenise(code);

    for (std::string token : tokens)
    {
        if (token == "\n")
        {
            token = "\\n";
        }
        log_text += "[";
        log_text += token;
        log_text += "] ";
        std::clog << "[" << token << "]\n";
    }

    bool result = analyse(tokens);

    if (result)
    {
        std::clog << "\nErroneus\n";
    }
    else
    {
        std::clog << "\nNo errors\n";
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

        // Creates the text box when the user will input the /path/to/file
        text_box((Rectangle){50, 50, 200, 150}, "Enter path to input file", "", "Confirm", file_path);

        // Creates the button
        button compile_button((Rectangle){50, 200, 200, 50}, "Transpile!", compile, std::string(file_path));

        scrollable_text_box((Rectangle){400, 50, 400, 500}, log_text + analysis::errors);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}