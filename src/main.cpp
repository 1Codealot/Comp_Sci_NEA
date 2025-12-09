#include "graphics.hpp"

int main(){
    // Initial GUI setup
    InitWindow(950,950,"OCR Reference Language Transpiler");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}