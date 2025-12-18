#include <string>
#include <vector>
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
            if(tokens_in.at(i) == "\""){
                holder += "\"";
                i++;
                while (tokens_in.at(i) != "\"")
                {
                    holder += tokens_in.at(i);
                    holder += " ";
                    i++;
                }
                holder = holder.substr(0, holder.size()-1);
                holder += "\"";
                new_tokens.push_back(holder);
                continue;                
            }


            new_tokens.push_back(tokens_in.at(i));
        }
        return new_tokens;
    }
};
std::vector<std::string> tokenise(std::string code)
{
    return tokeniser::stage2(tokeniser::stage1(code));
}