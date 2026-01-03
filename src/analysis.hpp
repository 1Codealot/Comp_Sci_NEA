#include <string>
#include <vector>

namespace analysis
{
    // Master string where all error messages are stored.
    std::string errors = "\n";

    /*
    Stage 1 of syntax analysis. This is where the transpiler checks that all the brackets are closed properly
    tokens = fully tokenised tokens
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

        for (size_t i = 0; i < tokens.size(); i++)
        {
            // Check "parity" (i.e checking that there is an equal amount of opens and closes)
            if (tokens.at(i) == "(")
            {
                normal_count++;
            }
            if (tokens.at(i) == ")")/* code */
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
            }
            if (tokens.at(i) == "[")
            {
                last_square = '[';
            }
            if (tokens.at(i) == ")")
            {
                last_normal = ')';

                if (last_square == '[')
                {
                    errors += "Mismatched brackets found; trying to close a normal without first closing the square brackets\n";
                    has_error = true;
                }
            }
            if (tokens.at(i) == "]")
            {
                last_square = ']';

                if (last_normal == '(')
                {
                    errors += "Mismatched brackets found; trying to close a square without first closing the normal brackets\n";
                    has_error = true;
                }
            }
        }

        return has_error;
    }

} // namespace analysis