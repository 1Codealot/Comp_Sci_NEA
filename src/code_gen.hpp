#include <string>
#include <vector>
#include <algorithm>

/*
A simple search and replace function, essentially just takes the bits of code that have a very simple 1:1
replacement and changes them in the tokens.
&tokens = a reference to the list of tokenised tokens (a reference is used as I can then change the elements
within the function, instead of making a new list and returning it).
*/
void search_and_replace(std::vector<std::string> &tokens)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {

        if (tokens.at(i) == "real")
        {
            tokens.at(i) = "float";
            continue;
        }

        else if (tokens.at(i) == "MOD")
        {
            tokens.at(i) = " % ";
            continue;
        }
        else if (tokens.at(i) == "DIV")
        {
            tokens.at(i) = "//";
            continue;
        }
        else if (tokens.at(i) == "AND")
        {
            tokens.at(i) = "and ";
            continue;
        }
        else if (tokens.at(i) == "OR")
        {
            tokens.at(i) = "or ";
            continue;
        }
        else if (tokens.at(i) == "NOT")
        {
            tokens.at(i) = " not ";
            continue;
        }
        else if (tokens.at(i) == "^")
        {
            tokens.at(i) = "**";
            continue;
        }
        else if (tokens.at(i) == "then")
        {
            tokens.at(i) = ":";
            continue;
        }
        else if (tokens.at(i) == "ASC")
        {
            tokens.at(i) = "ord";
            continue;
        }
        else if (tokens.at(i) == "CHR")
        {
            tokens.at(i) = "chr";
        }
        else if (tokens.at(i) == "upper")
        {
            tokens.at(i) = "upper()";
            continue;
        }
        else if (tokens.at(i) == "lower")
        {
            tokens.at(i) = "lower()";
            continue;
        }
        else if (tokens.at(i) == "readLine")
        {
            tokens.at(i) = "readline";
            continue;
        }
        else if (tokens.at(i) == "writeLine")
        {
            tokens.at(i) = "write";
            continue;
        }
    }
}

/*
The code generation function.
This is where the tokens get turned from OCR Reference language to actual Python
tokens = list of fully tokenised tokens
returns translated Python code
*/
std::string gen_code(std::vector<std::string> tokens)
{
    int tab_count = 0;
    std::string output_code = "#TODO CREATE OCR RANDOM FUNC\nfrom random import randint\nimport os";

    std::vector<std::string> list_of_exceptions = {"const", "real", "for", "do", "until", "elseif", "else", "switch", "default",
                                                   "open", "newFile", "endOfFile", "array", "[", "procedure", "function", "random", "print", "\n",
                                                   "MOD", "DIV", "OR", "AND", "NOT", "^", "endfunction", "endprocedure", "endif",
                                                   "endwhile", "then", "while", "next", "left", "right", "upper", "lower", "substring",
                                                   "length", "ASC", "CHR", "case", "endswitch", "write"};
    std::vector<std::string> scope_starts = {"if", "elseif", "else", "for", "while", "do", "switch", "function", "procedure"};
    std::vector<std::string> scope_ends = {"endif", "next", "endwhile", "until", "endswitch", "endfunction", "endprocedure", "elseif", "else"};

    search_and_replace(tokens);

    for (size_t i = 0; i < tokens.size(); i++)
    {
        std::cout << i << "\n";
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
                tab_count++;
                output_code += "match ";
                continue;
            }

            else if (tokens.at(i) == "case")
            {
                output_code.pop_back();
                output_code += "case ";
                continue;
            }
            else if (tokens.at(i) == "endswitch")
            {
                output_code.pop_back();
                tab_count--;
                continue;
            }
            else if (tokens.at(i) == "default")
            {
                output_code.pop_back();
                output_code += "case _"; // official way of designating a default in python according to PEP 0636 [12]
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
                int return_to_index = i;
                output_code += "while ";
                while (tokens.at(i) != "\n")
                {
                    i++;
                }
                tokens.insert(tokens.begin()+i,":");
                i = return_to_index;
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

                i += 2; // Go to token after the =
                std::string start = tokens.at(i);
                while (tokens.at(i) != "to")
                {
                    i++;
                    start += tokens.at(i);
                }
                i++; // Skip past "to".

                // Remove "to"
                start.pop_back();
                start.pop_back();

                std::string end;
                while (tokens.at(i) != "step" && tokens.at(i) != "\n")
                {
                    end += tokens.at(i);
                    i++;
                }

                std::string step = "";

                if (tokens.at(i) == "step")
                {
                    while (tokens.at(i) != "\n")
                    {
                        i++;
                        step += tokens.at(i);
                    }
                    // Remove new line
                    step.pop_back();
                    i--;
                }
                else
                {
                    step = "1";
                    i--;
                }

                output_code += "for " + ident + " in range(" + start + ",(" + end + ")+1," + step + "):";
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
            else if (tokens.at(i) == "length")
            {
                output_code += "__len__()";
                continue;
            }
            else if (tokens.at(i) == "do")
            {
                output_code += "while True: ";
                continue;
            }
            else if (tokens.at(i) == "until")
            {
                std::string break_condition = "";
                while (tokens.at(i) != "\n")
                {
                    i++;
                    break_condition += tokens.at(i);
                }
                break_condition.pop_back();

                output_code += "if (" + break_condition + "): break";
                i--; // to go back to the \n token
                continue;
            }
            else if (tokens.at(i) == "open")
            {
                output_code += "open";
                int brackets = 0;

                do
                {
                    i++;
                    output_code += tokens.at(i);
                    if (tokens.at(i) == "(")
                    {
                        brackets++;
                    }
                    if (tokens.at(i) == ")")
                    {
                        brackets--;
                    }

                } while (brackets != 0);

                output_code.pop_back();        // Remove the )
                output_code += ", mode='r+')"; // So that the line is in the style of f = open("file.txt", mode='r+')

                continue;
            }
            else if (tokens.at(i) == "newFile")
            {
                output_code += "try: open";
                int brackets = 0;

                do
                {
                    i++;
                    output_code += tokens.at(i);
                    if (tokens.at(i) == "(")
                    {
                        brackets++;
                    }
                    if (tokens.at(i) == ")")
                    {
                        brackets--;
                    }

                } while (brackets != 0);

                i++; // Past the )

                output_code.pop_back(); // Remove the )
                output_code += ", mode='x')\n";

                for (size_t n = 0; n < tab_count; n++)
                {
                    output_code += "\t";
                }

                output_code += "except FileExistsError:pass"; // Yes, really bad practice; but this is the least invasive way of doing it
                continue;
            }
            else if (tokens.at(i) == "write")
            {
                output_code += "write";
                int brackets = 0;
                do
                {
                    i++;
                    output_code += tokens.at(i);
                    if (tokens.at(i) == "(")
                    {
                        brackets++;
                    }
                    if (tokens.at(i) == ")")
                    {
                        brackets--;
                    }

                } while (brackets != 0);

                output_code.pop_back(); // Remove the )
                output_code += " + '\\n')";
                continue;
            }
            else if (tokens.at(i) == "endOfFile")
            {
                /* Yes this assumes that the file identifier is only one token which does mean
                that if they choose a file object inside a list, this will not work. However,
                it is a trivial fix, so it isn't worth trying to figure it out here.*/
                std::string file_ident = tokens.at(i - 2);
                //                      So that the () is required.
                output_code += "tell" + tokens.at(i+1 ) + tokens.at(i+2) + " == os.fstat(" + file_ident + ".fileno()).st_size";
                i+=2;
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