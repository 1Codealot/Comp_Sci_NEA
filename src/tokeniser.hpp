#include <string>
#include <vector>

std::vector<std::string> stage1(std::string input_code)
{
    std::vector<std::string> tokens;
    std::string current_token = "";
    const std::string special_chars = "1234567890-=+*/:\";,.'()\n";

    for (size_t i = 0; i < input_code.size(); i++)
    {
        if (input_code.at(i) == ' ')
        {
            if (current_token != "")
            {
                tokens.push_back(current_token);
            }
            current_token = "";
            continue;
        }

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