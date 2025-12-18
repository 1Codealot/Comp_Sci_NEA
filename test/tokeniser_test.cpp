#include <iostream>
#include <fstream>
#include <streambuf>
#include "../src/tokeniser.hpp"

int main(int argc, char const *argv[])
{
    // For taking in the file via the command line
    if (argc != 2)
    {
        std::cout << "Invalid usage.\n";
        return 1;
    }

    std::ifstream code_in(argv[1]);
    std::string code((std::istreambuf_iterator<char>(code_in)), std::istreambuf_iterator<char>());

    std::cout << code << "\n";

    std::vector<std::string> tokens;

    tokens = tokeniser::stage1(code);

    for (std::string token : tokens)
    {
        if (token == "\n") // To make the tokens easier to read
        {
            token = "\\n";
        }
        std::cout << "[" << token << "] "; // Once again making the tokens easier to read.
    }

    std::cout << std::endl;

    return 0;
}
