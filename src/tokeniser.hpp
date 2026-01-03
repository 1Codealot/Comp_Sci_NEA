#include <string>
#include <vector>
#include <regex>
namespace tokeniser // So thaat in future I can tell where functions come from.
{
    /* The first stage of tokenisation where all the big words are maade into individual tokens and every special chaaracter is a token
    input_code = code to be tokenised.
    */
    std::vector<std::string> stage1(std::string input_code)
    {
        std::vector<std::string> tokens;
        std::string current_token = "";
        // Numbers are not here because I want them to be part of tokens that may be identifers
        const std::string special_chars = "-=+*/:\";,.'()\n";
        // Goes through every character int the string.
        for (size_t i = 0; i < input_code.size(); i++)
        {
            // Removed the space check as it would mess with strings.

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
    /*
    The second stage of tokenisation. This is where comments are removed and strings are joined together and
    where blank tokens are removed.
    tokens_in = the vector of tokens made by the stage1 function.
    */
    std::vector<std::string> stage2(std::vector<std::string> tokens_in)
    {
        std::vector<std::string> new_tokens;
        for (size_t i = 0; i < tokens_in.size(); i++)
        {
            // Remove comments
            // "//" will be used for line comments "/*" ... "*/" will be used for multi line.
            if (tokens_in.at(i) == "/" && tokens_in.at(i + 1) == "/")
            {
                while (tokens_in.at(i) != "\n")
                {
                    i++;
                }
            }

            if (tokens_in.at(i) == "/" && tokens_in.at(i + 1) == "*")
            {
                while (!(tokens_in.at(i) == "*" && tokens_in.at(i + 1) == "/"))
                {
                    i++;
                }
                i += 2;
            }

            // Join together strings
            std::string holder = "";
            if (tokens_in.at(i) == "\"")
            {
                holder += "\"";
                i++;
                while (tokens_in.at(i) != "\"")
                {
                    holder += tokens_in.at(i);
                    i++;
                }
                holder += "\"";
                new_tokens.push_back(holder);
                continue;
            }

            new_tokens.push_back(tokens_in.at(i));
        }

        for (int i = 0; i < new_tokens.size(); i++)
        {
            if (new_tokens.at(i) == "\n")
            {
                continue;
            }
            // Using regex to replace leading and trailing spaces in a token
            new_tokens.at(i) = std::regex_replace(new_tokens.at(i), std::regex("^\\s"), "");
            new_tokens.at(i) = std::regex_replace(new_tokens.at(i), std::regex("\\s$"), "");

            // Delete blank tokens
            if (new_tokens.at(i) == "")
            {
                new_tokens.erase(new_tokens.begin() + i);
                i--;
            }
        }
        return new_tokens;
    }
};

/*
Master tokeniser function.
code = the code in to be tokenised
*/
std::vector<std::string> tokenise(std::string code)
{
    return tokeniser::stage2(tokeniser::stage1(code));
}