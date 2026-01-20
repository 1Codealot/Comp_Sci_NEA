#include <iostream>
#include <fstream>
#include "graphics.hpp"
#include "tokeniser.hpp"
#include "analysis.hpp"
#include "code_gen.hpp"

std::string log_text;

// So that the user can define their own output directory.
std::string default_out_path = "./out/";

void compile(std::string path_to_input_file)
{
    log_text = ""; // Reset the log
    std::ifstream input_file(path_to_input_file);

    // for some reason doing input_file.bad() exists but doesn't do what I expect it to do...
    if (!input_file.good())
    {
        log_text += "file: " + path_to_input_file + " could not be opened.\n";
    }

    std::string code((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

    std::vector<std::string> tokens = tokenise("\n\n" + code + "\n\n");

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

    bool erroneous = analyse(tokens);

    if (erroneous)
    {
        log_text += "\n\nCould not transpile.\n";
        return;
    }

    std::string python_code;
    python_code = gen_code(tokens);

    if (python_code == "")
    {
        log_text += "\n\nCould not translate code.\n";
        return;
    }

    std::ofstream output_file((default_out_path + "out.py").c_str());

    output_file << python_code << "\n";
    output_file.close();
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