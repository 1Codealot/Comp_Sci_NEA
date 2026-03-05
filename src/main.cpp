#include <iostream>
#include <fstream>
#include "graphics.hpp"
#include "tokeniser.hpp"
#include "analysis.hpp"
#include "code_gen.hpp"

std::string log_text;

// So that the user can define their own output directory.
char output_filename[max_text_size] = "./out/out.py";

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

    int index = 0;
    for (std::string token : tokens)
    {
        if (token == "\n")
        {
            token = "\\n";
        }
        log_text += "[";
        log_text += token;
        log_text += "] ";
        std::clog << index << ": [" << token << "]\n";
        index++;
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

    // Create the file and path
    std::string file_dir = std::string(output_filename);

#ifndef _WIN32
    file_dir = file_dir.substr(0, file_dir.find_last_of('/')); // Not windows
#else
    file_dir = file_dir.substr(0, file_dir.find_last_of('\\')); // Windows version
#endif

    file_dir = "mkdir -p " + file_dir;

    std::system(file_dir.c_str());

    std::ofstream output_file(output_filename);

    output_file << python_code << "\n";
    output_file.close();
}

char file_path[max_text_size] = "\0";

int main()
{
    bool input_or_ouput_path_edit = true; // true => input; false => output
    std::string toggle_button_text;
    startWindow();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        if (input_or_ouput_path_edit)
        {
            // Creates the text box when the user will input the /path/to/file
            text_box((Rectangle){50, 50, 200, 150}, "Enter path to input file", "", "Confirm", file_path);
            toggle_button_text = "Press to edit output file path.";
            // Creates the compile button
            button compile_button((Rectangle){50, 200, 200, 50}, "Transpile!", compile, std::string(file_path));
        }
        else
        {
            // Creates a text box for the output path name.
            text_box((Rectangle){50, 50, 200, 150}, "Enter output file name", "", "Confirm", output_filename);
            toggle_button_text = "Press to edit input file path.";
        }

        // Button for switching between getting text for the output path or for the input path.
        if (GuiButton((Rectangle){50, 300, 200, 50}, toggle_button_text.c_str()))
        {
            input_or_ouput_path_edit = !input_or_ouput_path_edit; // I.E. changes the state.
        }

        scrollable_text_box((Rectangle){400, 50, 400, 500}, log_text + analysis::errors);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}