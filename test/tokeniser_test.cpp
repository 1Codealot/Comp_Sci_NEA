#include <iostream>
#include <fstream>
#include "../src/tokeniser.hpp"

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        std::cout << "Invalid usage.\n";
        return 1;
    }


    std::cout << argv[1] << "\n";
    std::string code;
    std::fstream code_in(argv[2]);

    while (std::getline(code_in, code));

    std::vector<std::string> tokens;

    tokens = stage1(code);

    for (std::string token : tokens)
    {
        std::cout << "[" << token << "] ";
    }

    std::cout << std::endl;

    return 0;
}
