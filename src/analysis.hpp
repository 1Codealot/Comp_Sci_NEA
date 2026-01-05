#include <string>
#include <vector>

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

} // namespace analysis

/*
Master analysis function
tokens = code that has been tokenised
returns if any of the stages have errors because if there are syntax errors, we cannot continue onto code geneeration.
*/
bool analyse(std::vector<std::string> tokens)
{
    return analysis::stage1(tokens) || analysis::stage2(tokens) /*|| analysis::stage3(tokens) /*|| analysis::stage4(tokens)
    /*|| analysis::stage5(tokens) /*|| analysis::stage6(tokens) /*|| analysis::stage7(tokens)*/
        ;
}