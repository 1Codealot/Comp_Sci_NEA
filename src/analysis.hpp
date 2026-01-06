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
                std::cout << "Identifier = " << prev_token << "\n";
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

} // namespace analysis

/*
Master analysis function
tokens = code that has been tokenised
returns if any of the stages have errors because if there are syntax errors, we cannot continue onto code geneeration.
*/
bool analyse(std::vector<std::string> tokens)
{
    return analysis::stage1(tokens) || analysis::stage2(tokens) || analysis::stage3(tokens) /*|| analysis::stage4(tokens)
    /*|| analysis::stage5(tokens) /*|| analysis::stage6(tokens) /*|| analysis::stage7(tokens)*/
        ;
}