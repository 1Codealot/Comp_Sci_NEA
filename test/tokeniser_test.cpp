#include <iostream>
#include <fstream>
#include <streambuf>
#include "../src/tokeniser.hpp"

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        std::cout << "Invalid usage.\n";
        return 1;
    }

    std::cout << argv[1] << "\n";
    std::ifstream code_in(argv[1]);
    std::string code((std::istreambuf_iterator<char>(code_in)), std::istreambuf_iterator<char>());

    std::cout << code << "\n";

    std::vector<std::string> tokens;

    tokens = stage1(code);

    for (std::string token : tokens)
    {
        if (token == "\n")
        {
            token = "\\n";
        }
        std::cout << "[" << token << "] ";
    }

    std::cout << std::endl;

    return 0;
}
