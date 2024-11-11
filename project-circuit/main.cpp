/*******************************************************************************************
*
*   raylib [textures] example - Texture source and destination rectangles
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include <iostream>
#include "raylib.h"
#include "raymath.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight, "raylib [textures] examples - texture source and destination rectangles");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    Texture2D resistorTexture = LoadTexture("./Resistor.png");

    int frameWidth = resistorTexture.width;
    int frameHeight = resistorTexture.height;

    // Source rectangle (part of the texture to use for drawing)
    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };

    // Destination rectangle (screen rectangle where drawing part of texture)
    Rectangle destRec = { screenWidth / 2.0f, screenHeight / 2.0f, 80, 80 };

    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    Vector2 origin = { 0, 40 };

    float rotation = 0;
    bool drawExtentions = false;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rotation = 180.0f-(Vector2Angle(Vector2Subtract(Vector2{ destRec.x, destRec.y }, GetMousePosition()), Vector2{ 1,0 }) * 180.0f / PI);

        Vector2 ResistorVector = Vector2Subtract(GetMousePosition(), Vector2{ destRec.x, destRec.y });
        float lineLength = (Vector2Length(ResistorVector) - destRec.width) / 2.0f;

        if (Vector2Length(ResistorVector) > destRec.width) {
            drawExtentions = true;
            origin.x = -lineLength;
        }
        else {
            drawExtentions = false;
            origin = { 0, 40 };
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // NOTE: Using DrawTexturePro() we can easily rotate and scale the part of the texture we draw
        // sourceRec defines the part of the texture we use for drawing
        // destRec defines the rectangle where our texture part will fit (scaling it to fit)
        // origin defines the point of the texture used as reference for rotation and scaling
        // rotation defines the texture rotation (using origin as rotation point)

        DrawTexturePro(resistorTexture, sourceRec, destRec, origin, rotation, GREEN);
        if (drawExtentions) {
            DrawLineEx({ destRec.x, destRec.y }, Vector2Add({ destRec.x, destRec.y }, Vector2Scale(Vector2Normalize(ResistorVector), lineLength)), 5.0f, SKYBLUE);
            DrawLineEx(Vector2Subtract(GetMousePosition(), Vector2Scale(Vector2Normalize(ResistorVector), lineLength)), GetMousePosition(), 5.0f, SKYBLUE);
        }




        DrawLine((int)destRec.x, 0, (int)destRec.x, screenHeight, GRAY);
        DrawLine(0, (int)destRec.y, screenWidth, (int)destRec.y, GRAY);

        DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    UnloadTexture(resistorTexture);

    // De-Initialization
    //--------------------------------------------------------------------------------------

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
