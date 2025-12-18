#include <string>
#include <vector>

/* The first stage of tokenisation where all the big words are maade into individual tokens and every special chaaracter is a token
input_code = code to be tokenised.
*/
std::vector<std::string> stage1(std::string input_code)
{
    std::vector<std::string> tokens;
    std::string current_token = "";
    const std::string special_chars = "1234567890-=+*/:\";,.'()\n";

    // Goes through every character int the string.
    for (size_t i = 0; i < input_code.size(); i++)
    {
        // Adding the current token when there is a space.
        if (input_code.at(i) == ' ')
        {
            if (current_token != "") // Anti-blank mechanism.
            {
                tokens.push_back(current_token);
            }
            current_token = "";
            continue;
        }

        // Adds special char acters as a token.
        if (special_chars.contains(input_code.at(i)))
        {
            if (current_token != "")
            {

                tokens.push_back(current_token);
            }
            current_token = "";
            tokens.push_back(input_code.substr(i, 1));
            continue;
        }

        current_token += input_code.at(i);
    }

    return tokens;
}