#include <raylib.h>
#include "globals.h"
#include<string>

#define DEBUG_PRINT 1

#if DEBUG_PRINT
#include<chrono>
//macro to calculate execution time of given function
#define execFunctionWithTimeMeasure(f, var) \
auto startTime = std::chrono::high_resolution_clock::now();\
f;\
std::chrono::duration<double, std::milli> miliseconds = std::chrono::high_resolution_clock::now() - startTime;\
var=  miliseconds.count();

#else //DEBUG_PRINT
#define execFunctionWithTimeMeasure(f, var) f

#endif //DEBUG_PRINT

MainRes_t mainRes; // global container for stuff
static void init(char* argv[]);
static void processInput();


int main(int argc, char* argv[])
{
    double simulationTime{0.0}, renderTime{ 0.0 }; //debug variables
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
        processInput();
        //simulate
        if (mainRes.simulate)
        {
            execFunctionWithTimeMeasure(mainRes.sand->simulate(), simulationTime);
        }
        //end simulate
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        execFunctionWithTimeMeasure(mainRes.sand->render() , renderTime);
        mainRes.ui->render();
        mainRes.drawingArea->render();
        DrawText((std::to_string(mainRes.sand->pixels).c_str()), mainRes.options->screenWidth - 80, mainRes.options->screenHeight - 50,
            16, BLACK);

#if DEBUG_PRINT
        DrawText(std::to_string(renderTime).c_str(), 80, mainRes.options->screenHeight - 40, 16, BLACK);
        DrawText(std::to_string(simulationTime).c_str(), 80, mainRes.options->screenHeight - 80, 16, BLACK);
#endif //DEBUG_PRINT

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
    mainRes.color = RED;
    mainRes.simulate = false;
    mainRes.drawingArea = new DrawingArea(upperRightDrawingArea, height, width);
    mainRes.sand = new Sand(upperRightDrawingArea, height, width);
    mainRes.ui = new Ui(upperRightUi, height, mainRes.options->screenWidth * 0.08f);
}

static void processInput()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        if (mainRes.simulate)
        {
            mainRes.simulate = false;
        }
        else
        {
            mainRes.simulate = true;
        }
    }
    if (IsKeyPressed(KEY_ONE))
    {
        mainRes.sand->generateSand({ 0,0 }, mainRes.color, 500);
    }
    if (IsKeyPressed(KEY_TWO))
    {
        mainRes.sand->generateSand({ 0,0 }, mainRes.color, 1000);
    }
    if (IsKeyPressed(KEY_THREE))
    {
        mainRes.sand->generateSand({ 0,0 }, mainRes.color, 2000);
    }
    if (IsKeyPressed(KEY_R))
    {
        mainRes.sand->resetTable();
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        const Vector2 mousePosition = GetMousePosition();
        //check if mouse is in drawing range
        if (mainRes.drawingArea->isMouseOnObj(mousePosition))
        {
            mainRes.sand->generateSand(mousePosition, mainRes.color, 8);
        }
        else if (mainRes.ui->isMouseOnObj(mousePosition))
        {
            mainRes.ui->getColorFromGradient(mousePosition, &mainRes.color);
            mainRes.ui->getBlackFromGradient(mousePosition, &mainRes.color);
        }
    }
}
