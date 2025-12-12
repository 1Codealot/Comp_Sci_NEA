#include <iostream>
#include <fstream>
#include "graphics.hpp"

// Dummy procedure just to demonstrate that I can run a function on a button press.
void compile(std::string path_to_input_file)
{
    std::ifstream input_file(path_to_input_file);
    std::string tmp;
    while (std::getline(input_file, tmp))
    {
        std::cout << tmp << std::endl;
    }
}


char file_path[max_text_size] = "\0";
char long_text[max_text_size] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec est nunc, sodales nec neque quis, pretium varius purus. Aliquam sollicitudin magna non ex interdum, a bibendum felis efficitur. \n\n\nVestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Duis gravida ipsum eget nisi posuere imperdiet. Duis sollicitudin feugiat maximus. Proin molestie pulvinar quam. Ut sit amet ante enim. Quisque sit amet cursus elit. \n\nCurabitur ornare lacinia massa, at semper elit imperdiet vitae. Nulla facilisi. Morbi nisl odio, interdum eu porta nec, elementum at urna. Mauris volutpat convallis enim, tincidunt tincidunt turpis pretium a. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus auctor aliquet diam eu eleifend. Sed lacinia tortor at orci eleifend ullamcorper. \nMauris tristique nibh pretium, blandit lacus quis, faucibus sem.Phasellus felis augue, suscipit vitae placerat et, sollicitudin sed elit. Cras at posuere lectus, ullamcorper rutrum mauris. Proin pellentesque odio eget ex egestas, quis congue tortor pulvinar. Morbi sed diam quis turpis sodales finibus. In eleifend felis non neque vehicula dictum. Duis porta molestie lacus et luctus. Ut ullamcorper auctor libero, ac aliquam felis. Donec turpis orci, dignissim vel lorem quis, sagittis finibus quam. Duis id aliquet urna, vitae sagittis urna. ";

int main()
{
    startWindow();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        // Creates the text box when the user will input the /path/to/file
        text_box((Rectangle){50, 50, 200, 150}, "Enter path to input file", "", "Confirm", file_path);

        // Creates the button
        button compile_button((Rectangle){50, 200, 200, 50}, "Transpile!", compile, std::string(file_path));

        scrollable_text_box((Rectangle){400,50, 400,500}, long_text);

        EndDrawing();

    }

    CloseWindow();
    return 0;
}