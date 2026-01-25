#include <string>
#include <vector>
#include <algorithm>

std::string gen_code(std::vector<std::string> tokens)
{
    int tab_count = 0;
    std::string output_code = "#TODO CREATE OCR RANDOM FUNC\nfrom random import randint";

    std::vector<std::string> list_of_exceptions = {"const", "real", "for", "do", "elseif", "else", "switch", "default",
                                                   "open", "newFile", "array", "[", "procedure", "function", "random", "print", "\n",
                                                   "MOD", "DIV", "OR", "AND", "NOT", "^", "endfunction", "endprocedure", "endif",
                                                   "endwhile", "then", "while", "next", "left", "right", "upper", "lower", "substring",
                                                   "length", "ASC", "CHR"};
    std::vector<std::string> scope_starts = {"if", "elseif", "else", "for", "while", "do", "switch", "case", "function", "procedure"};
    std::vector<std::string> scope_ends = {"endif", "next", "endwhile", "until", "endswitch", "endfunction", "endprocedure", "elseif", "else"};

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (std::find(scope_starts.begin(), scope_starts.end(), tokens.at(i)) != scope_starts.end())
        {
            tab_count++;
        }
        if (std::find(scope_ends.begin(), scope_ends.end(), tokens.at(i)) != scope_ends.end())
        {
            tab_count--;
            if (tab_count < 0)
            {
                // This means error occured in code generation
                return "";
            }
        }
        if (std::find(list_of_exceptions.begin(), list_of_exceptions.end(), tokens.at(i)) != list_of_exceptions.end())
        {
            // First ones that are trivial replacements
            if (tokens.at(i) == "const" || tokens.at(i) == "endfunction" || tokens.at(i) == "endprocedure" || tokens.at(i) == "endif" || tokens.at(i) == "endwhile")
            {
                // Just ignore
                continue;
            }
            else if (tokens.at(i) == "next")
            {
                i++;
                continue;
            }
            else if (tokens.at(i) == "real")
            {
                output_code += "float";
                continue;
            }
            else if (tokens.at(i) == "elseif")
            {
                output_code.pop_back(); // to remove a tab
                output_code += "elif ";
                continue;
            }
            else if (tokens.at(i) == "else")
            {
                output_code.pop_back(); // to remove a tab
                output_code += "else:";
                continue;
            }
            else if (tokens.at(i) == "switch")
            {
                output_code += "match ";
                continue;
            }
            else if (tokens.at(i) == "default")
            {
                output_code += "case _:"; // official way of designating a default in python according to PEP 0636 [12]
                continue;
            }
            else if (tokens.at(i) == "MOD")
            {
                output_code += " % ";
                continue;
            }
            else if (tokens.at(i) == "DIV")
            {
                output_code += "//";
                continue;
            }
            else if (tokens.at(i) == "AND")
            {
                output_code += "and ";
                continue;
            }
            else if (tokens.at(i) == "OR")
            {
                output_code += "or ";
                continue;
            }
            else if (tokens.at(i) == "NOT")
            {
                output_code += "not ";
                continue;
            }
            else if (tokens.at(i) == "^")
            {
                output_code += "**";
                continue;
            }
            else if (tokens.at(i) == "then")
            {
                output_code += ":";
                continue;
            }
            else if (tokens.at(i) == "ASC")
            {
                output_code += "ord";
                continue;
            }
            else if (tokens.at(i) == "CHR")
            {
                output_code += "chr";
            }
            else if (tokens.at(i) == "upper")
            {
                output_code += "upper()";
                continue;
            }
            else if (tokens.at(i) == "lower")
            {
                output_code += "lower()";
                continue;
            }

            // Non-trivial replacements.
            else if (tokens.at(i) == "\n")
            {
                output_code += "\n";

                for (size_t j = 0; j < tab_count; j++)
                {
                    output_code += "\t";
                }
                continue;
            }
            else if (tokens.at(i) == "procedure" || tokens.at(i) == "function")
            {
                output_code += "def ";

                while (tokens.at(i) != ")")
                {
                    i++;
                    output_code += tokens.at(i);
                }
                output_code += ":";
                continue;
            }
            else if (tokens.at(i) == "while")
            {
                output_code += "while ";
                while (tokens.at(i) != "\n")
                {
                    i++;
                    output_code += tokens.at(i) + " ";
                }
                output_code.pop_back();
                output_code.pop_back();
                output_code += ":";
                i--;
                continue;
            }
            else if (tokens.at(i) == "print")
            {
                int brackets = 0;
                output_code += tokens.at(i);
                do
                {
                    i++;
                    if (tokens.at(i) == "(")
                    {
                        brackets++;
                    }
                    if (tokens.at(i) == ")")
                    {
                        brackets--;
                    }

                    output_code += tokens.at(i);
                } while (brackets != 0);
                output_code.pop_back();
                output_code += ", end = \"\")";
                continue;
            }
            else if (tokens.at(i) == "for")
            {
                i++;
                std::string ident = tokens.at(i);
                i += 2;
                // TODO: This assumes that the boundary definitions are 1 token long.
                std::string start = tokens.at(i);
                i += 2;
                std::string end = tokens.at(i);

                std::string step = "1";
                i++;

                if (tokens.at(i) == "step")
                {
                    i++;
                    step = tokens.at(i);
                }
                else
                {
                    i--;
                }

                output_code += "for " + ident + " in range(" + start + "," + end + "+1," + step + "):";
                continue;
            }
            else if (tokens.at(i) == "left")
            {
                output_code.pop_back();
                output_code.pop_back();
                output_code += "[:";
                i += 2;
                while (tokens.at(i) != ")")
                {
                    output_code += tokens.at(i) + " ";
                    i++;
                }
                output_code += "]";
                continue;
            }
            else if (tokens.at(i) == "right")
            {
                output_code.pop_back();
                output_code.pop_back();
                output_code += "[-(";
                i += 2;
                while (tokens.at(i) != ")")
                {
                    output_code += tokens.at(i) + " ";
                    i++;
                }
                output_code += "):]";
                continue;
            }
            else if (tokens.at(i) == "substring")
            {
                output_code.pop_back();
                output_code.pop_back();
                i += 2;

                int brackets = 0;

                std::string start;
                while (tokens.at(i) != "," || brackets > 0)
                {
                    start += tokens.at(i);
                    if (tokens.at(i) == "(")
                    {
                        brackets++;
                    }
                    else if (tokens.at(i) == ")")
                    {
                        brackets--;
                    }
                    i++;
                }
                i++;
                std::string end;
                while (tokens.at(i) != ")" || brackets > 0)
                {
                    end += tokens.at(i);
                    if (tokens.at(i) == "(")
                    {
                        brackets++;
                    }
                    else if (tokens.at(i) == ")")
                    {
                        brackets--;
                    }
                    i++;
                }

                output_code += "[(" + start + "):(" + end + ")+(" + start + ")]"; 
                continue;
            }

            else
            {
                std::cout << tokens.at(i) << "\n";
                return "";
            }
        }
        else
        {
            output_code += tokens.at(i) + " ";
        }
    }

    return output_code;
}