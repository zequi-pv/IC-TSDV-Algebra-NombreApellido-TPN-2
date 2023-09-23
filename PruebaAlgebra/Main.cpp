#include <iostream>
#include <raylib.h>
#include <raymath.h>

using namespace std;

//Hola

void CameraHandler(Camera3D& camera, int& cameraMode);
void GetMagnitude(Vector3 endPos, float& magnitude);
void GetFirstPerpendicular(Vector3 endPos, Vector3& perpendicular);
void NormalizeVector(Vector3& vector);
void SetNewMagnitude(Vector3& vector, float designedMagnitude);
void CrossProduct(Vector3 vector1, Vector3 vector2, Vector3 vertex, Vector3& vectorPerp);
void GetVertical(Vector3 endPos, Vector3& vertical);
void DrawInstructions();
void DrawPyramid(Vector3 startPos, Vector3 vectorA, Vector3 vectorB, Vector3 vectorC, float magnitudeC, float userInput);
float CalculateScalarProduct(Vector3 vector1, Vector3 vector2);

void main()
{
    Vector2 magnitudeTextPos = { 10, 30 };
    Vector2 vectorATextPos = { 10, 50 };
    Vector2 vectorBTextPos = { 10, 70 };
    Vector2 vectorCTextPos = { 10, 90 };

    Vector3 startPos = { 0.0f, 0.0f, 0.0f };

    Vector3 vectorA = { (float)GetRandomValue(2, 8), (float)GetRandomValue(2, 8), (float)GetRandomValue(2, 8)}; //Vector A 
    Vector3 vectorB = { 0.0f, 0.0f, 0.0f }; //Vector B
    Vector3 vectorC = { 0.0f, 0.0f, 0.0f }; //Vector C

    float userInput = 0;

    float magnitudeA = 1; //A magnitude
    float magnitudeB = 1; //B magnitude
    float magnitudeC = 1; //C magnitude

    bool starting = true;

    SetExitKey(27);
    InitWindow(800, 450, "Algebra TP2");

    Model model;

    Camera camera = { 0 };
    camera.position = { 0.0f, 2.0f, 4.0f };    // Camera position
    camera.target = { 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 100.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;

    camera.position = startPos;

    camera.position.x += 2.0f;
    camera.position.y += 2.0f;
    camera.position.z += 2.0f;

    camera.target = startPos;

    DisableCursor();

    SetTargetFPS(60);

    GetMagnitude(vectorA, magnitudeA); //A magnitude
    GetFirstPerpendicular(vectorA, vectorB); //Crea vector B

    SetNewMagnitude(vectorB, magnitudeA); //Normaliza y multiplica magnitud Vector B por la magnitud Vector A
    GetMagnitude(vectorB, magnitudeB); //Magnitud actual de Vector B

    //GetVertical(vectorA, vectorC); //Crea Vector C
    CrossProduct(vectorA, vectorB, startPos, vectorC);

    cout << "\nMagnitud A: " << magnitudeA << endl;
    cout << "\nMagnitud B: " << magnitudeB << endl;

    while (!WindowShouldClose())
    {
        CameraHandler(camera, cameraMode);
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("Vector A and B magnitude: %.2f", magnitudeA), magnitudeTextPos.x, magnitudeTextPos.y, 15, BLACK);
        DrawText("Vector A: RED", vectorATextPos.x, vectorATextPos.y, 15, BLACK);
        DrawText("Vector B: GREEN", vectorBTextPos.x, vectorBTextPos.y, 15, BLACK);
        DrawText("Vector C: BLUE", vectorCTextPos.x, vectorCTextPos.y, 15, BLACK);

        int key;

        BeginMode3D(camera);

        DrawLine3D(startPos, vectorA, RED); //Dibuja Vector A
        DrawLine3D(startPos, vectorB, GREEN); //Dibuja Vector B
        DrawLine3D(startPos, vectorC, BLUE); //Dibuja Vector C

        DrawPyramid(startPos, vectorA, vectorB, vectorC, magnitudeC, userInput);

        EndMode3D();

        if (starting)
        {
            DrawInstructions();

            key = GetCharPressed() - 48;

            if (key > 1 && key < 10)
            {
                SetNewMagnitude(vectorC, magnitudeA / key); //Normaliza el Vector C y le da la magnitud deseada por el usuario
                GetMagnitude(vectorC, magnitudeC); //Vector C magnitude actual

                cout << "\nInput: " << key << endl;
                cout << "\nMagnitud C: " << magnitudeC << endl;

                userInput = (float)key;

                starting = false;
            }  
        }
        else
        {
            if (IsKeyDown('Z'))
            {
                camera.target = startPos;
            }
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
}

void CameraHandler(Camera3D& camera, int& cameraMode)
{
    if (IsKeyPressed(KEY_F))
    {
        cameraMode = CAMERA_FREE;
        camera.up = { 0.0f, 1.0f, 0.0f };
    }

    if (IsKeyPressed(KEY_G))
    {
        cameraMode = CAMERA_FIRST_PERSON;
        camera.up = { 0.0f, 1.0f, 0.0f };
    }

    if (IsKeyPressed(KEY_H))
    {
        cameraMode = CAMERA_THIRD_PERSON;
        camera.up = { 0.0f, 1.0f, 0.0f };
    }

    if (IsKeyPressed(KEY_J))
    {
        cameraMode = CAMERA_ORBITAL;
        camera.up = { 0.0f, 1.0f, 0.0f };
    }

    if (IsKeyPressed(KEY_P))
    {
        if (camera.projection == CAMERA_PERSPECTIVE)
        {
            cameraMode = CAMERA_THIRD_PERSON;
            camera.position = { 0.0f, 2.0f, -100.0f };
            camera.target = { 0.0f, 2.0f, 0.0f };
            camera.up = { 0.0f, 1.0f, 0.0f };
            camera.projection = CAMERA_ORTHOGRAPHIC;
            camera.fovy = 20.0f; 
        }
        else if (camera.projection == CAMERA_ORTHOGRAPHIC)
        {
            cameraMode = CAMERA_THIRD_PERSON;
            camera.position = { 0.0f, 2.0f, 10.0f };
            camera.target = { 0.0f, 2.0f, 0.0f };
            camera.up = { 0.0f, 1.0f, 0.0f };
            camera.projection = CAMERA_PERSPECTIVE;
            camera.fovy = 60.0f;
        }
    }

    UpdateCamera(&camera, cameraMode);
}

float CalculateScalarProduct(Vector3 vector1, Vector3 vector2)
{
    float scalarProduct = (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z);

    return scalarProduct;
}

void GetMagnitude(Vector3 endPos, float& magnitude)
{
    magnitude = sqrt(pow(endPos.x, 2.0f) + pow(endPos.y, 2.0f) + pow(endPos.z, 2.0f));
}

void GetFirstPerpendicular(Vector3 endPos, Vector3& perpendicular)
{
    perpendicular.x = endPos.z;
    perpendicular.z = -1 * endPos.x;
}

void NormalizeVector(Vector3& vector)
{
    float vectorMagnitude;

    GetMagnitude(vector, vectorMagnitude);

    vector.x /= vectorMagnitude;
    vector.y /= vectorMagnitude;
    vector.z /= vectorMagnitude;
}

void SetNewMagnitude(Vector3& vector, float designedMagnitude)
{
    NormalizeVector(vector);

    vector.x *= designedMagnitude;
    vector.y *= designedMagnitude;
    vector.z *= designedMagnitude;
}

void CrossProduct(Vector3 vector1, Vector3 vector2, Vector3 vertex, Vector3& vectorPerp)
{
    vectorPerp.x = ((vector1.y * vector2.z) - (vector1.z * vector2.y)) + vertex.x;
    vectorPerp.y = ((vector1.z * vector2.x) - (vector1.x * vector2.z)) + vertex.y;
    vectorPerp.z = ((vector1.x * vector2.y) - (vector1.y * vector2.x)) + vertex.z;
}

void GetVertical(Vector3 endPos, Vector3& vertical)
{
    vertical.y = endPos.x;
    vertical.x = -1 *endPos.y;
}

void DrawInstructions()
{
    int recPosX = GetScreenWidth() / 3 * 2 - 40;
    int recPosY = GetScreenHeight() / 12;
    int recWidth = GetScreenWidth() / 3 - 20;
    int recHeight = GetScreenHeight() / 12 * 10;
    Color recColor = BLACK;

    int textX = recPosX + 10;
    int fontSize = 18;
    Color textColor = RAYWHITE;

    int textY1 = recPosY + 10;
    int textY2 = recPosY + 30;
    int textY3 = recPosY + 70;
    int textY4 = recPosY + 110;
    int textY5 = recPosY + 140;
    int textY6 = recPosY + 170;
    int textY7 = recPosY + 200;
    int textY8 = recPosY + 230;
    int textY9 = recPosY + 260;
    int textY10 = recPosY + 290;
    int textY11 = recPosY + 320;

    DrawRectangle(recPosX, recPosY, recWidth, recHeight, recColor);

    DrawText("Choose a value for n to", textX, textY1, fontSize, textColor);
    DrawText("set Vector C magnitude:", textX, textY2, fontSize, textColor);
    DrawText("(1/n * Vector A magnitude)", textX, textY3, fontSize, textColor);

    DrawText("Press key 2 = 1/2", textX, textY4, fontSize, textColor);
    DrawText("Press key 3 = 1/3", textX, textY5, fontSize, textColor);
    DrawText("Press key 4 = 1/4", textX, textY6, fontSize, textColor);
    DrawText("Press key 5 = 1/5", textX, textY7, fontSize, textColor);
    DrawText("Press key 6 = 1/6", textX, textY8, fontSize, textColor);
    DrawText("Press key 7 = 1/7", textX, textY9, fontSize, textColor);
    DrawText("Press key 8 = 1/8", textX, textY10, fontSize, textColor);
    DrawText("Press key 9 = 1/9", textX, textY11, fontSize, textColor);
}

void DrawPyramid(Vector3 startPos, Vector3 vectorA, Vector3 vectorB, Vector3 vectorC, float magnitudeC, float userInput)
{
    Vector3 xMovement; //Es el desplazamiento para cada piso en x
    xMovement.x = vectorA.x / userInput;
    xMovement.y = vectorA.y / userInput;
    xMovement.z = vectorA.z / userInput;

    Vector3 yMovement;
    yMovement.x = vectorB.x / userInput;
    yMovement.y = vectorB.y / userInput;
    yMovement.z = vectorB.z / userInput;

    //...
    Vector3 upRight = Vector3Add(xMovement, yMovement);
    Vector3 upLeft = Vector3Add(Vector3Scale(xMovement, -1.0f), yMovement);
    Vector3 downRight = Vector3Subtract(xMovement, yMovement);
    Vector3 downLeft = Vector3Subtract(Vector3Scale(xMovement, -1.0f), yMovement);

    //Auxiliares para no modificar los valores originales
    Vector3 zeroToLine = startPos;
    Vector3 auxA = vectorA;
    Vector3 auxB = vectorB;
    Vector3 floatPoint = Vector3Add(vectorA, vectorB);

    float floorQnty = userInput / 2; //Cantidad de veces que se repetir� el for que dibuja los pisos. 

    for (int i = 0; i < floorQnty; i++)
    {
        DrawLine3D(Vector3Add(zeroToLine, (Vector3Scale(upRight, i))), Vector3Add(Vector3Add(zeroToLine, (Vector3Scale(upRight, i))), vectorC), PINK);
        DrawLine3D(Vector3Add(auxA, (Vector3Scale(upLeft, i))), Vector3Add(Vector3Add(auxA, (Vector3Scale(upLeft, i))), vectorC), PINK);
        DrawLine3D(Vector3Add(auxB, (Vector3Scale(downRight, i))), Vector3Add(Vector3Add(auxB, (Vector3Scale(downRight, i))), vectorC), PINK);
        DrawLine3D(Vector3Add(floatPoint, (Vector3Scale(downLeft, i))), Vector3Add(Vector3Add(floatPoint, (Vector3Scale(downLeft, i))), vectorC), PINK);

        DrawLine3D(Vector3Add(zeroToLine, Vector3Scale(upRight, i)), Vector3Add(auxA, Vector3Scale(upLeft, i)), VIOLET);
        DrawLine3D(Vector3Add(zeroToLine, Vector3Scale(upRight, i)), Vector3Add(auxB, Vector3Scale(downRight, i)), VIOLET);
        DrawLine3D(Vector3Add(floatPoint, Vector3Scale(downLeft, i)), Vector3Add(auxA, Vector3Scale(upLeft, i)), VIOLET);
        DrawLine3D(Vector3Add(floatPoint, Vector3Scale(downLeft, i)), Vector3Add(auxB, Vector3Scale(downRight, i)), VIOLET);

        zeroToLine = Vector3Add(zeroToLine, vectorC);
        auxA = Vector3Add(auxA, vectorC);
        auxB = Vector3Add(auxB, vectorC);
        floatPoint = Vector3Add(floatPoint, vectorC);

        DrawLine3D(Vector3Add(zeroToLine, Vector3Scale(upRight, i)), Vector3Add(auxA, Vector3Scale(upLeft, i)), VIOLET);
        DrawLine3D(Vector3Add(zeroToLine, Vector3Scale(upRight, i)), Vector3Add(auxB, Vector3Scale(downRight, i)), VIOLET);
        DrawLine3D(Vector3Add(floatPoint, Vector3Scale(downLeft, i)), Vector3Add(auxA, Vector3Scale(upLeft, i)), VIOLET);
        DrawLine3D(Vector3Add(floatPoint, Vector3Scale(downLeft, i)), Vector3Add(auxB, Vector3Scale(downRight, i)), VIOLET);
    }
}

