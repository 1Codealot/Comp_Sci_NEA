#include <string>
#include <vector>
#include <algorithm>
#include <deque> // using this as a stack

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
            if (current_token == "if" || current_token == "while" || current_token == "for" || current_token == "do" ||
                current_token == "switch" || current_token == "function" || current_token == "procedure" || current_token == "for")
            {
                nest_count++;
            }
            if (current_token == "endif" || current_token == "endwhile" || current_token == "next" || current_token == "until" ||
                current_token == "endswitch" || current_token == "endfunction" || current_token == "endprocedure" || current_token == "next")
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
                if (!(std::regex_match(prev_token, std::regex("[A-Za-z_](\\w)*"))))
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
    I was initially planning to allow subprocedures within sbporcedures, as Python does allow this.
    However I could not get this to work.
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

                // Check that the name of the subprocedure is usable.
                if (std::find(protected_identifers.begin(), protected_identifers.end(), tokens.at(i)) != protected_identifers.end())
                {
                    has_error = true;
                    errors += "you cannot use " + tokens.at(i) + " as an identifier again.\n";
                    continue;
                }

                if (!(std::regex_match(tokens.at(i), std::regex("[A-Za-z_](\\w)*"))))
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
                    continue;
                }

                int open_bracket_index = i;
                while (tokens.at(i) != ")")
                {
                    // So that parameters can go over several lines
                    if (tokens.at(i) == "\n")
                    {
                        i++;
                    }

                    if (i == tokens.size() - 1)
                    {
                        has_error = true;
                        // Yes, this will be somewhat tested for in stage1(), however this is here
                        // so that the program doesn't crash.
                        errors += "No ')' found when defining parameters\n";
                        break;
                    }

                    if (i - open_bracket_index > 100)
                    {
                        has_error = true;
                        errors += "Too many parameters found in a subprocedure definition.\n";
                        break;
                    }

                    i++;
                    if (tokens.at(i) == "\n")
                    {
                        i++;
                    }

                    if (tokens.at(i) == ")")
                    {
                        break;
                    }

                    if (std::find(protected_identifers.begin(), protected_identifers.end(), tokens.at(i)) != protected_identifers.end())
                    {
                        has_error = true;
                        errors += "you cannot use " + tokens.at(i) + " as a parameter identifier.\n";
                    }
                    if (!(std::regex_match(tokens.at(i), std::regex("[A-Za-z_](\\w)*"))))
                    {
                        has_error = true;
                        errors += tokens.at(i) + " is not a valid name for a parameter.\n";
                    }

                    i++;
                    if (tokens.at(i) == "\n")
                    {
                        i++;
                    }

                    if (tokens.at(i) != "," && tokens.at(i) != ")")
                    {
                        has_error = true;
                        errors += "expected ','. Found " + tokens.at(i) + "\n";
                    }
                    else if (tokens.at(i) == ")")
                    {
                        break;
                    }
                }

                if (is_function)
                {
                    // Checking that there is a return statement in the function.
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

    /*
    Stage 5 of syntax analysis. This is where I check that all if/elseif/else blocks are
    written correctly.
    tokens = fully tokenised tokens
    returns whether or not there is an error.
    */
    bool stage5(std::vector<std::string> tokens)
    {
        bool has_error = false;
        int endifs_needed = 0;

        for (size_t i = 0; i < tokens.size(); i++)
        {
            if (tokens.at(i) == "if")
            {
                endifs_needed++;
                i++;
                if (tokens.at(i) == "then")
                {
                    has_error = true;
                    errors += "if statement found with no condition\n";
                }

                while (tokens.at(i) != "then" || tokens.at(i) != "\n")
                {
                    if (tokens.at(i) == "\n")
                    {
                        has_error = true;
                        errors += "No 'then' found when defining if statement.\n";
                        break;
                    }
                    else if (tokens.at(i) == "then")
                    {
                        break;
                    }
                    else
                    {
                        i++;
                    }
                }
                while (endifs_needed != 0)
                {
                    if (i == tokens.size() - 1)
                    {
                        has_error = true;
                        errors += "If statement found without endif\n";
                        break;
                    }

                    // For nested if statements
                    if (tokens.at(i) == "if")
                    {
                        endifs_needed++;
                        i++;
                        if (tokens.at(i) == "then")
                        {
                            has_error = true;
                            errors += "if statement found with no condition\n";
                        }
                        while (tokens.at(i) != "then" || tokens.at(i) != "\n")
                        {
                            if (tokens.at(i) == "\n")
                            {
                                has_error = true;
                                errors += "No 'then' found when defining if statement.\n";
                                break;
                            }
                            else if (tokens.at(i) == "then")
                            {
                                break;
                            }
                            else
                            {
                                i++;
                            }
                        }
                    }

                    if (tokens.at(i) == "elseif")
                    {
                        i++;
                        if (tokens.at(i) == "then")
                        {
                            has_error = true;
                            errors += "elseif found without condition.\n";
                        }
                        while (tokens.at(i) != "then" || tokens.at(i) != "\n")
                        {
                            if (tokens.at(i) == "\n")
                            {
                                errors += "No 'then' found when defining elseif statement.\n";
                                break;
                            }
                            else if (tokens.at(i) == "then")
                            {
                                break;
                            }
                            else
                            {
                                i++;
                            }
                        }
                    }
                    if (tokens.at(i) == "endif")
                    {
                        endifs_needed--;
                    }

                    i++;
                }
            }
        }
        return has_error;
    }

    /*
    Stage 6 of syntax analysis. This is where I check that all switch/case blocks are
    written correctly.
    tokens = fully tokenised tokens
    returns whether or not there is an error.
    */
    bool stage6(std::vector<std::string> tokens)
    {
        bool has_error = false;
        for (size_t i = 0; i < tokens.size(); i++)
        {
            if (tokens.at(i) == "case")
            {
                has_error = true;
                errors += "case found without switch statement preceding it\n";
            }

            if (tokens.at(i) == "default")
            {
                has_error = true;
                errors += "default found without switch statement preceding it\n";
            }

            bool found_default = false;
            if (tokens.at(i) == "switch")
            {
                i++;
                if (tokens.at(i) == ":")
                {
                    has_error = true;
                    errors += "expected an expression after 'switch'; found ':'\n";
                }

                while (tokens.at(i) != ":")
                {
                    i++;
                    if (tokens.at(i) == "case" || tokens.at(i) == "default" || tokens.at(i) == "endswitch")
                    {
                        has_error = true;
                        errors += "expected : after switch statement, found " + tokens.at(i) + "\n";
                        break;
                    }
                }

                while (tokens.at(i) != "endswitch")
                {
                    if (i == tokens.size() - 1)
                    {
                        has_error = true;
                        errors += "switch statement found with no endswitch.";
                        break;
                    }
                    i++;

                    if (tokens.at(i) == "case")
                    {
                        if (found_default)
                        {
                            has_error = true;
                            errors += "case statement found after default\n";
                        }

                        i++;
                        if (tokens.at(i) == ":")
                        {
                            has_error = true;
                            errors += "case statement found with no condition after\n";
                        }

                        while (tokens.at(i) != ":")
                        {
                            i++;
                            if (tokens.at(i) == "case" || tokens.at(i) == "default" || tokens.at(i) == "endswitch")
                            {
                                has_error = true;
                                errors += "expected : after case statement, found " + tokens.at(i) + "\n";
                                break;
                            }
                        }
                    }
                    if (tokens.at(i) == "default")
                    {
                        if (found_default)
                        {
                            has_error = true;
                            errors += "More than one default found.\n";
                        }

                        found_default = true;
                        i++;

                        if (tokens.at(i) != ":")
                        {
                            has_error = true;
                            errors += "Expected ':' after default; found " + tokens.at(i) + "\n";
                        }
                    }
                }
            }
        }

        return has_error;
    }

    /*
    Stage 7 of syntax analysis. This is where I check that all while and do-until blocks are
    written correctly.
    tokens = fully tokenised tokens
    returns whether or not there is an error.
    */
    bool stage7(std::vector<std::string> tokens)
    {
        bool has_error = false;

        for (size_t i = 0; i < tokens.size(); i++)
        {
            // While loop logic
            if (tokens.at(i) == "endwhile")
            {
                has_error = true;
                errors += "endwhile found without matching while.\n";
            }

            if (tokens.at(i) == "while")
            {
                while (tokens.at(i) != "endwhile")
                {
                    i++;
                    if (i == tokens.size() - 1)
                    {
                        has_error = true;
                        errors += "while loop with no endwhile found\n";
                        break;
                    }
                }
            }

            // Do-until loop logic.
            if (tokens.at(i) == "until")
            {
                has_error = true;
                errors += "until found without matching do.\n";
            }

            if (tokens.at(i) == "do")
            {
                i++;
                if (tokens.at(i) != "\n")
                {
                    has_error = true;
                    errors += "expected a newline after do statement, found" + tokens.at(i) + "\n";
                }
                while (tokens.at(i) != "until")
                {
                    i++;
                    if (i == tokens.size() - 2)
                    {
                        has_error = true;
                        errors += "do block found without ending until statement\n";
                        break;
                    }
                }

                i++;
                if (tokens.at(i) == "\n")
                {
                    has_error = true;
                    errors += "Expected a statement after until, but found a newline instead.\n";
                }
            }
        }

        return has_error;
    }

    /*
    Stage 8 of syntax analysis. This is where I check that all for loop blocks are
    written correctly.
    tokens = fully tokenised tokens
    returns whether or not there is an error.
    */
    bool stage8(std::vector<std::string> tokens)
    {
        bool has_error = false;

        /*
        This is a container of type dequeue (double ended queue) which I will use as
        a stack by only using the `dequeue::push_back` and `dequeue::pop_back` as there is
        no stack type inplemented into the C++ standard library.
        */
        std::deque<std::string> iterator_identifiers_stack;

        for (size_t i = 0; i < tokens.size(); i++)
        {
            if (tokens.at(i) == "to" || tokens.at(i) == "step" || tokens.at(i) == "next")
            {
                has_error = true;
                errors += "found " + tokens.at(i) + " before for.\n";
            }

        for_checker:
        {
            if (tokens.at(i) == "for")
            {
                i++;
                if (!(std::regex_match(tokens.at(i), std::regex("[A-Za-z_](\\w)*"))))
                {
                    has_error = true;
                    errors += "invalid identifier for itereator found: " + tokens.at(i) + "\n";
                }

                if (std::find(protected_identifers.begin(), protected_identifers.end(), tokens.at(i)) != protected_identifers.end())
                {
                    has_error = true;
                    errors += "you cannot use " + tokens.at(i) + " as an iterator identifier.\n";
                }

                iterator_identifiers_stack.push_back(tokens.at(i));

                i++;
                if (tokens.at(i) != "=")
                {
                    has_error = true;
                    errors += "expected '=', found " + tokens.at(i) + "\n";
                }

                // Interesting way of essentially making a while loop, while also iterating i
                for (; tokens.at(i) != "to"; ++i)
                {
                    if (i == tokens.size() - 2 || tokens.at(i) == "next")
                    {
                        has_error = true;
                        errors += "no 'to' found after for statement\n";
                        break;
                    }

                    if (tokens.at(i) == "step")
                    {
                        has_error = true;
                        errors += "found 'step' before finding 'to'\n";
                        break;
                    }
                }

                for (; iterator_identifiers_stack.size() != 0; ++i)
                {
                    if (tokens.at(i) == "for")
                    {
                        goto for_checker;
                    }

                    else if (i == tokens.size() - 1)
                    {
                        has_error = true;
                        errors += "for loop found with no 'next'\n";
                        break;
                    }
                    if (tokens.at(i) == "next")
                    {
                        i++;
                        if (tokens.at(i) != iterator_identifiers_stack.back())
                        {
                            has_error = true;
                            errors += "identifier " + tokens.at(i) + " does not match expected identifier " + iterator_identifiers_stack.back() + "\n";
                        }
                        else
                        {
                            iterator_identifiers_stack.pop_back();
                        }
                    }
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
    analysis::errors = "\n";

    bool result = false;
    result = result | analysis::stage1(tokens);
    result = result | analysis::stage2(tokens);
    result = result | analysis::stage3(tokens);
    result = result | analysis::stage4(tokens);
    result = result | analysis::stage5(tokens);
    result = result | analysis::stage6(tokens);
    result = result | analysis::stage7(tokens);
    result = result | analysis::stage8(tokens);
    return result;
}