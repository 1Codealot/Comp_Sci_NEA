#include <string>
#include <vector>
#include <algorithm>

std::string gen_code(std::vector<std::string> tokens)
{
    int tab_count = 0;
    std::string output_code = "#TODO CREATE OCR RANDOM FUNC\nfrom random import randint";

    std::vector<std::string> list_of_exceptions = {"const", "real", "for", "do", "if", "elseif", "switch", "open",
                                                   "newFile", "array", "[", "procedure", "function", "random", "print", "\\n"};
    std::vector<std::string> scope_starts = {"if", "for", "while", "do", "switch", "case", "function", "procedure"};
    std::vector<std::string> scope_ends = {"endif", "next", "endwhile", "until", "endswitch", "endfunction", "endprocedure"};

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (std::find(scope_starts.begin(), scope_starts.end(), tokens.at(i)) != scope_starts.end())
        {
            tab_count++;
        }
        else if (std::find(scope_ends.begin(), scope_ends.end(), tokens.at(i)) != scope_ends.end())
        {
            tab_count--;
            if (tab_count < 0)
            {
                // This means error occured in code generation
                return "";
            }
        }
        else if (std::find(list_of_exceptions.begin(), list_of_exceptions.end(), tokens.at(i)) != list_of_exceptions.end())
        {
            // TODO
        }
        else
        {
            output_code += tokens.at(i);
        }
    }

    return output_code;
}