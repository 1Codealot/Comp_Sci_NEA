#include <string>
#include <vector>

std::vector<std::string> stage1(std::string input_code)
{
    std::vector<std::string> tokens;
    std::string current_token = "";
    const std::string special_chars = "1234567890-=+*/:\";,.()\n ";

    for (char current_char : input_code)
    {
        if (special_chars.contains(current_char))
        {
            tokens.push_back(current_token);
            if (current_char != ' ')
            {
                tokens.push_back("" + current_char); // Interesting hack to add just a character into a std::vector that expects a std::string.
            }
            continue;
        }
    }

    return tokens;
}