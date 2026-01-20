#include <string>
#include <vector>
#include <algorithm>

std::string gen_code(std::vector<std::string> tokens)
{
    int tab_count = 0;
    std::string output_code = "#TODO CREATE OCR RANDOM FUNC\nfrom random import randint";

    std::vector<std::string> list_of_exceptions = {"const", "real", "for", "do", "if", "elseif", "else", "switch", "default",
                                                   "open", "newFile", "array", "[", "procedure", "function", "random", "print", "\\n",
                                                   "MOD", "DIV", "OR", "AND", "NOT", "^"};
    std::vector<std::string> scope_starts = {"if", "elseif", "else", "for", "while", "do", "switch", "case", "function", "procedure"};
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
            // First ones that are trivial replacements
            if (tokens.at(i) == "const")
            {
                // Just ignore
                continue;
            }
            else if (tokens.at(i) == "real")
            {
                output_code += "float";
                continue;
            }
            else if (tokens.at(i) == "else")
            {
                output_code += "else:";
                continue;
            }
            else if (tokens.at(i) == "switch")
            {
                output_code += "match";
                continue;
            }
            else if (tokens.at(i) == "default")
            {
                output_code += "case _:"; // official way of designating a default in python according to PEP 0636 [12]
                continue;
            }
            else if (tokens.at(i) == "procedure" || tokens.at(i) == "function")
            {
                output_code += "def";
                continue;
            }
            else if (tokens.at(i) == "\\n")
            {
                output_code += "\n";

                for (size_t j = 0; j < tab_count; j++)
                {
                    output_code += "\t";
                }

                continue;
            }
            else if (tokens.at(i) == "MOD")
            {
                output_code += "%";
                continue;
            }
            else if (tokens.at(i) == "DIV")
            {
                output_code += "//";
                continue;
            }
            else if (tokens.at(i) == "AND")
            {
                output_code += "and";
                continue;
            }
            else if (tokens.at(i) == "OR")
            {
                output_code += "or";
                continue;
            }
            else if (tokens.at(i) == "NOT")
            {
                output_code += "not";
                continue;
            }
            else if (tokens.at(i) == "^")
            {
                output_code += "**";
                continue;
            }
            else
            {
                return "";
            }
        }
        else
        {
            output_code += tokens.at(i);
        }
    }

    return output_code;
}