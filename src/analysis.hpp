#include <string>
#include <vector>
#include <algorithm>

/*
The list of identifiers that are not allowed to be used or reused.
This is outside the functions that it is used in as we cannot have a function and variable have the exact same name.
Due to how I have programmed this, this will recieve variable identifiers first which may make it confusing as to why
it says that a variable already has the name even if the function is defined first.
However, it should and will still error anyway.
*/
std::vector<std::string> protected_identifers =
    {"AND", "OR", "NOT", "MOD", "DIV", "const", "global", "input", "print", "str", "int",
     "float", "real", "bool", "for", "to", "step", "next", "while", "endwhile", "do", "until", "if", "then", "elseif", "else", "endif",
     "switch", "case", "default", "endswitch", "ASC", "CHR", "open", "newFile", "array", "procedure", "endprocedure", "function",
     "return", "endfunction", "random"};

namespace analysis
{
    // Master string where all error messages are stored.
    std::string errors = "\n";

    /*
    Stage 1 of syntax analysis. This is where the transpiler checks that all the brackets are closed properly
    tokens = fully tokenised tokens
    returns whether or not there is an error
    */
    bool stage1(std::vector<std::string> tokens)
    {
        bool has_error = false;

        std::string opens = "([";
        std::string closers = ")]";

        // Increment on open found; Decrement on close found
        int normal_count = 0;
        int square_count = 0;

        char last_normal = ')';
        char last_square = ']';
        char last_open = 0;

        for (size_t i = 0; i < tokens.size(); i++)
        {
            // Check "parity" (i.e checking that there is an equal amount of opens and closes)
            if (tokens.at(i) == "(")
            {
                normal_count++;
            }
            if (tokens.at(i) == ")")
            {
                normal_count--;
            }
            if (tokens.at(i) == "[")
            {
                square_count++;
            }
            if (tokens.at(i) == "]")
            {
                square_count--;
            }
        }

        if (normal_count > 0)
        {
            errors += "Unbalanced brackets; too many open brackets without closers (" + std::to_string(normal_count) + " too many)\n";
            has_error = true;
        }
        if (normal_count < 0)
        {
            errors += "Unbalanced brackets; too many close brackets without opens (" + std::to_string(-normal_count) + " too many)\n";
            has_error = true;
        }
        if (square_count > 0)
        {
            errors += "Unbalanced square brackets; too many open brackets without closers (" + std::to_string(square_count) + " too many)\n";
            has_error = true;
        }
        if (square_count < 0)
        {
            errors += "Unbalanced square brackets; too many close brackets without opens (" + std::to_string(-square_count) + " too many)\n";
            has_error = true;
        }

        // check that a situation like (] or [) doesn't happen
        for (size_t i = 0; i < tokens.size(); i++)
        {
            if (tokens.at(i) == "(")
            {
                last_normal = '(';
                last_open = '(';
            }
            if (tokens.at(i) == "[")
            {
                last_square = '[';
                last_open = '[';
            }
            if (tokens.at(i) == ")")
            {
                last_normal = ')';

                if (last_square == '[' && last_open == '[')
                {
                    errors += "Mismatched brackets found; trying to close a normal without first closing the square brackets\n";
                    has_error = true;
                }
            }
            if (tokens.at(i) == "]")
            {
                last_square = ']';

                if (last_normal == '(' && last_open == '(')
                {
                    errors += "Mismatched brackets found; trying to close a square without first closing the normal brackets\n";
                    has_error = true;
                }
            }
        }

        return has_error;
    }

    /*
    Stage 2 of syntax analysis. This is where the transpiler checks that all the nesting is correct.
    I realised that I don't need to check if all the condtion controllers or the loops need to be checked if they
    have their correct enders (e.g. checking if an if statment has an endif) as I do that in later stages so
    it is redundant.
    tokens = fully tokenised tokens
    returns whether or not there is an error
    */
    bool stage2(std::vector<std::string> tokens)
    {
        bool has_error = false;

        int nest_count = 0;

        for (size_t i = 0; i < tokens.size(); i++)
        {
            std::string current_token = tokens.at(i);
            if (current_token == "if" || current_token == "while" || current_token == "for" || current_token == "do")
            {
                nest_count++;
            }
            if (current_token == "endif" || current_token == "endwhile" || current_token == "next" || current_token == "until")
            {
                nest_count--;
            }
        }

        if (nest_count != 0)
        {
            has_error = true;
            errors += "Inbalanced nesting\n";
        }

        return has_error;
    }

    /*
    Stage 3 of syntax analysis. This is where the transpiler checks that all variables are defined properly.
    This includes checking if the identifier is allowed and that constants are not changed.
    tokens = fully tokenised tokens
    returns whether or not there is an error*/
    bool stage3(std::vector<std::string> tokens)
    {
        bool has_error = false;
        std::string current_token;
        std::string prev_token;
        std::string prev_prev_token;
        for (size_t i = 2; i < tokens.size() - 1; i++)
        {
            current_token = tokens.at(i);
            prev_token = tokens.at(i - 1);
            prev_prev_token = tokens.at(i - 2);
            if (current_token == "=" && tokens.at(i + 1) != "=")
            {
                if (std::find(protected_identifers.begin(), protected_identifers.end(), prev_token) != protected_identifers.end())
                {
                    has_error = true;
                    errors += "you cannot use " + prev_token + " as an identifier again.\n";
                    continue;
                }
                if (!(std::regex_match(prev_token, std::regex("\\D(\\w)*"))))
                {
                    has_error = true;
                    errors += prev_token + " is not a valid identifier name\n";
                    continue;
                }
                if (prev_prev_token == "const")
                {
                    protected_identifers.push_back(prev_token);
                }
            }
        }
        return has_error;
    }

    /*
    Stage 4 of syntax analysis. This is where I check that all subprocedures are defined
    correctly.
    I will allow defining subprocedures within subprocedures as this is allowed in python.
    As this is allowed in Python I will allow it to be transpiled.
    tokens = fully tokenised tokens
    returns whether or not there is a error.
    */
    bool stage4(std::vector<std::string> tokens)
    {
        bool has_error = false;

        for (size_t i = 0; i < tokens.size(); i++)
        {
            bool is_function;
            if (tokens.at(i) == "function" || tokens.at(i) == "procedure")
            {
                is_function = tokens.at(i) == "function";

                i++;

                if (std::find(protected_identifers.begin(), protected_identifers.end(), tokens.at(i)) != protected_identifers.end())
                {
                    has_error = true;
                    errors += "you cannot use " + tokens.at(i) + " as an identifier again.\n";
                    continue;
                }

                if (!(std::regex_match(tokens.at(i), std::regex("\\D(\\w)*"))))
                {
                    has_error = true;
                    errors += tokens.at(i) + "is not a valid subprocedure name.\n";
                    continue;
                }

                i++;

                // check parameters
                if (tokens.at(i) != "(")
                {
                    has_error = true;
                    errors += "expected a '(' after subprocedure name\n";
                }

                int open_bracket_index = i;
                while (tokens.at(i) != ")")
                {
                    if (i - open_bracket_index > 100)
                    {
                        has_error = true;
                        errors += "Too many parameters found in a subprocedure definition\n";
                    }

                    i++;
                    if (tokens.at(i) != ")")
                    {
                        break;
                    }
                    if (std::find(protected_identifers.begin(), protected_identifers.end(), tokens.at(i)) != protected_identifers.end())
                    {
                        has_error = true;
                        errors += "you cannot use " + tokens.at(i) + " as a parameter identifier.\n";
                    }
                    if (!(std::regex_match(tokens.at(i), std::regex("\\D(\\w)*"))))
                    {
                        has_error = true;
                        errors += tokens.at(i) + " is not a valid name for a parameter.\n";
                    }

                    if (tokens.at(i) != "," && tokens.at(i) != ")")
                    {
                        has_error = true;
                        errors += "expected ','. Found " + tokens.at(i);
                    }
                    i++;
                }

                if (is_function)
                {
                    bool found_return = false;
                    while (tokens.at(i) != "endfunction")
                    {

                        if (tokens.at(i) == "return")
                        {
                            found_return = true;
                        }


                        if (tokens.at(i) == "function" || tokens.at(i) == "procedure")
                        {
                            has_error = true;
                            errors += "You cannot define a subprocedure within a subprocedure.\n";
                        }

                        i++;
                    }
                    if (!found_return)
                    {
                        has_error = true;
                        errors += "function found missing return statement\n";
                    }
                }
                else
                {
                    while (tokens.at(i) != "endprocedure")
                    {

                        if (tokens.at(i) == "return")
                        {
                            has_error = true;
                            errors += "Unexpected return found in a procedure\n";
                        }
                        if (tokens.at(i) == "function" || tokens.at(i) == "procedure")
                        {
                            has_error = true;
                            errors += "You cannot define a subprocedure within a subprocedure.\n";
                        }

                        i++;
                    }
                }
            }
        }
        return has_error;
    }
} // namespace analysis

/*
Master analysis function
tokens = code that has been tokenised
returns if any of the stages have errors because if there are syntax errors, we cannot continue onto code geneeration.
*/
bool analyse(std::vector<std::string> tokens)
{
    return analysis::stage1(tokens) || analysis::stage2(tokens) || analysis::stage3(tokens) || analysis::stage4(tokens)
        /*|| analysis::stage5(tokens) /*|| analysis::stage6(tokens) /*|| analysis::stage7(tokens)*/
        ;
}