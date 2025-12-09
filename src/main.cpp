#include <iostream>
#include "graphics.hpp"

// Dummy procedure just to demonstrate that I can run a function on a button press.
void compile()
{
    std::cout << "Nothing to transpile yet..." << std::endl;
}

int main()
{
    startWindow();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        // Creates the button
        button compile_button((Rectangle){50, 150, 200, 50}, "Transpile!", compile);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}