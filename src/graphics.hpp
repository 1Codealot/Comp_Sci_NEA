#include <functional>
#include <string>
#include "../include/raylib/raylib/include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../include/raylib/raygui/include/raygui.h"

void startWindow()
{
    // Initialises the window.
    InitWindow(950, 950, "OCR Reference Language Transpiler");
    SetTargetFPS(60);
}

// Class definition for buttons
class button
{
public:
    button(Rectangle rect, std::string text, std::function<void()> on_press);                                 // Constructor function signature
    button(Rectangle rect, std::string text, std::function<void(std::string)> on_press, std::string str_arg); // Constructor function signature
};

/* Constructor defnition
rect = Rectangular bounding area for the button.
text = text to show on the button
on_press = name of function to run when the button is pressed. This funtion must be of return type `void` and have no arguments.
*/
button::button(Rectangle rect, std::string text, std::function<void()> on_press)
{
    if (GuiButton(rect, text.c_str()))
    {
        on_press();
    }
}

button::button(Rectangle rect, std::string text, std::function<void(std::string)> on_press, std::string str_arg)
{

    if (GuiButton(rect, text.c_str()))
    {
        on_press(str_arg);
    }
}

constexpr int max_text_size = 2048;

int text_box(Rectangle rect, std::string title, std::string msg, std::string labels, char *input_text)
{
    return GuiTextInputBox(rect, title.c_str(), msg.c_str(), labels.c_str(), input_text, max_text_size, NULL);
}
