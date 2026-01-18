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
    // This uses a C++ feature known as overloading which allows me to create multiple functions with the smae nambe, but different types
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
/* Constructor defnition
rect = Rectangular bounding area for the button.
text = text to show on the button.
on_press = name of function to run when the button is pressed. This funtion must be of return type `void` and have one argument of type std::string.
str_arg = the string to be passed as an arguement to the passed in funtion.
*/
button::button(Rectangle rect, std::string text, std::function<void(std::string)> on_press, std::string str_arg)
{

    if (GuiButton(rect, text.c_str()))
    {
        on_press(str_arg);
    }
}

// Maximum length of a C-style string.
constexpr int max_text_size = 2048;

/* Defines the text box used.
rect = Rectangular bounding area for the text input box.
title = tilte of the text input box.
labels = labels of the buttons used in the text input box, delimited by ';' to denote a new button.
input_text = a pointer to the C-style string where teh contents of the text box will be sent to.
*/
int text_box(Rectangle rect, std::string title, std::string msg, std::string labels, char *input_text)
{
    return GuiTextInputBox(rect, title.c_str(), msg.c_str(), labels.c_str(), input_text, max_text_size, NULL);
}

int scroll_offset = 15;
/* Creates a text area that can be scrolled
rect = Rectangular bounding area for the text to be rendered in.
text = The text to be displayed.
*/
void scrollable_text_box(Rectangle rect, std::string text)
{
    const int scroll_speed = 15;
    const int char_size = 15;
    const int buffer = 7; // roughly char_size / 2
    int chars_across = 0;
    int lines_down = 0;

    // Draws the background.
    DrawRectangle(rect.x, rect.y, rect.width, rect.height + buffer, GRAY);

    for (char c : text)
    { // The text renderer will not do new lines correctly, so I must correct for that here.
        if (c == '\n')
        {
            lines_down++;
            chars_across = 0;
            continue;
        }

        // Character (not word) wrapping. I.E. if a letter would go out the bounding area, move it to a new line
        if ((chars_across * char_size) + rect.x > rect.x + (rect.width - char_size))
        {
            lines_down++;
            chars_across = 0;
        }

        // Check that the chars are in y range includeing scroll.
        if (rect.y + (lines_down * char_size) + (scroll_offset) < rect.y + rect.height && rect.y + (lines_down * char_size) + (scroll_offset) > rect.y)
        {
            DrawTextCodepoint(GetFontDefault(), c, {buffer + (chars_across * char_size) + rect.x, rect.y + scroll_offset + (lines_down * char_size) - buffer}, char_size, BLACK);
        }
        chars_across++;
    }

    lines_down -= 2;
    scroll_offset = std::max(-lines_down * char_size, std::min(char_size, (scroll_offset + (int)(GetMouseWheelMove() * scroll_speed))));
}