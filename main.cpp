#include <raylib.h>
#include "globals.h"

MainRes_t mainRes; // global container for stuff
static void init(char* argv[]);

int main(int argc, char* argv[])
{
    bool simulate = false;
    unsigned color = 1;
    init(argv);
    SetTargetFPS(60);
	InitWindow(mainRes.options->screenWidth, mainRes.options->screenHeight, "kolorki.");
    if (mainRes.options->fullscreen)
    {
        ToggleBorderlessWindowed();
    }

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        mainRes.frameCounter = (mainRes.frameCounter + 1) % mainRes.maxFps;

        //process input
        if (IsKeyPressed(KEY_SPACE))
        {
            if (simulate)
            {
                simulate = false;
            }
            else
            {
                simulate = true;
            }
        }
        if (IsKeyPressed(KEY_ONE))
        {
            color = 1;
        }
        if (IsKeyPressed(KEY_TWO))
        {
            color = 2;
        }
        if (IsKeyPressed(KEY_THREE))
        {
            color = 3;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            const Vector2 mousePosition = GetMousePosition();
            //check if mouse is in drawing range
            if (mainRes.drawingArea->isMouseOnObj(mousePosition))
            {
                mainRes.sand->generateSand(mousePosition, color,1);
            }
            //else if (mainRes.ui->isMouseOnObj(mousePosition))
            {

            }
        }
        // end processing input

        //simulate
        if (simulate)
        {
            mainRes.sand->simulate();
        }
        //end simulate
        BeginDrawing();
        mainRes.ui->render();
        mainRes.drawingArea->render();
        mainRes.sand->render();
        ClearBackground(RAYWHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
}

static void init(char* argv[])
{
    mainRes.options = new OptionParser(argv[0]);
    const unsigned fps{ 60 };
    const Vector2 upperRightDrawingArea{ mainRes.options->screenWidth*0.05f,mainRes.options->screenHeight * 0.05f };
    const Vector2 upperRightUi{ mainRes.options->screenWidth * 0.9f,mainRes.options->screenHeight * 0.05f };
    float width{ mainRes.options->screenWidth * 0.8f };
    float height{ mainRes.options->screenHeight * 0.9f };
    mainRes.maxFps = fps;
    mainRes.drawingArea = new DrawingArea(upperRightDrawingArea, height, width);
    mainRes.sand = new Sand(upperRightDrawingArea, height, width);
    mainRes.ui = new Ui(upperRightUi, height, mainRes.options->screenWidth * 0.08f);
    
}
